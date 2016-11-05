#include "stdafx.h"

#include "OneTweak.h"
#include "OneTweakDriver.h"

#include "MinHook.h"

OneTweakDriver::OneTweakDriver()
{
	PrintLog(__FUNCTION__);
	MH_Initialize();

	MH_CreateHook(::GetPrivateProfileIntA, HookGetPrivateProfileIntA, reinterpret_cast<void**>(&GetPrivateProfileIntA));
	MH_CreateHook(::CreateWindowExA, HookCreateWindowExA, reinterpret_cast<void**>(&CreateWindowExA));
	MH_CreateHook(::SetWindowPos, HookSetWindowPos, reinterpret_cast<void**>(&SetWindowPos));
	MH_CreateHookApiEx(L"dinput8.dll", "DirectInput8Create", HookDirectInput8Create, reinterpret_cast<void**>(&DirectInput8Create), NULL);
}

OneTweakDriver::~OneTweakDriver()
{
	PrintLog(__FUNCTION__);

	ShowCursor(true);

	if (oldWndProc)
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oldWndProc);

	MH_Uninitialize();
}

void OneTweakDriver::Run()
{
	MH_EnableHook(MH_ALL_HOOKS);
}

UINT WINAPI OneTweakDriver::HookGetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName)
{
	auto& config = g_Host->GetConfig();
	if (config.borderless.enabled)
	{
		// disable fullscreen
		if (strcmp(lpKeyName, "bFull Screen") == 0)
		{
			PrintLog("Setting \"bFull Screen\" = 0");
			return 0;
		}

		// set resolution
		int cx = config.borderless.RenderWidth ? config.borderless.RenderWidth : GetSystemMetrics(SM_CXSCREEN);
		if (cx > 0 && strcmp(lpKeyName, "iSize W") == 0)
		{
			PrintLog("Setting \"iSize W\" = %d", cx);
			return cx;
		}

		int cy = config.borderless.RenderHeight ? config.borderless.RenderHeight : GetSystemMetrics(SM_CYSCREEN);
		if (cy > 0 && strcmp(lpKeyName, "iSize H") == 0)
		{
			PrintLog("Setting \"iSize H\" = %d", cy);
			return cy;
		}

		int active = config.borderless.active;
		if (strcmp(lpKeyName, "bAlwaysActive") == 0)
		{
			PrintLog("Setting \"bAlwaysActive\" = %d", active);
			return active;
		}
	}

	return OneTweakDriver::GetInstance().GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpFileName);
}

HWND WINAPI OneTweakDriver::HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	PrintLog(__FUNCTION__);

	auto& config = g_Host->GetConfig();

	HWND hWnd = OneTweakDriver::GetInstance().CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	// Workaround crash, do not print/compare lpClassName/lpWindowName directly !
	std::unique_ptr<char[]> className(new char[MAX_PATH]);
	std::unique_ptr<char[]> windowName(new char[MAX_PATH]);

	GetClassNameA(hWnd, className.get(), MAX_PATH);
	GetWindowTextA(hWnd, windowName.get(), MAX_PATH);

	PrintLog("Window 0x%p: ClassName \"%s\", WindowName: \"%s\"", hWnd, className, windowName);

	if (strcmp(className.get(), config.ClassName.c_str()) == 0 && strcmp(windowName.get(), config.WindowName.c_str()) == 0)
	{
		PrintLog("Found game window: 0x%p", hWnd);
		OneTweakDriver::GetInstance().hWnd = hWnd;
		if (config.borderless.enabled)
		{
			LONG_PTR dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
			LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);

			PrintLog("Window dwStyle: 0x%08X", dwStyle);
			PrintLog("Window dwExStyle: 0x%08X", dwExStyle);

			config.borderless.flags ? dwStyle = config.borderless.flags : dwStyle &= ~WS_OVERLAPPEDWINDOW;
			config.borderless.flagsEx ? dwExStyle = config.borderless.flagsEx : dwExStyle &= ~(WS_EX_OVERLAPPEDWINDOW | WS_EX_TOPMOST);

			// SetWindowLongPtr fail when return == 0 and GetLastError() != 0, but does not clear last error when successed

			SetLastError(0);
			if (SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle) == 0 && GetLastError() != 0)
			{
				PrintLog("SetWindowLongPtr for dwStyle failed");
			}
			else
			{
				PrintLog("Borderless Window dwStyle: 0x%08X", dwStyle);

				SetLastError(0);
				if (SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwExStyle) == 0 && GetLastError() != 0)
				{
					PrintLog("SetWindowLongPtr for dwExStyle failed");
				}
				else
				{
					PrintLog("Borderless Window dwExStyle: 0x%08X", dwExStyle);
				}

				DWORD nWidth = config.borderless.WindowWidth ? config.borderless.WindowWidth : GetSystemMetrics(SM_CXSCREEN);
				DWORD nHeight = config.borderless.WindowHeight ? config.borderless.WindowHeight : GetSystemMetrics(SM_CYSCREEN);

				OneTweakDriver::GetInstance().SetWindowPos(hWnd, HWND_TOP, 0, 0, nWidth, nHeight, SWP_NOCOPYBITS | SWP_NOSENDCHANGING);
				PrintLog("Borderless fullscreen window mode active for window 0x%p", hWnd);
			}
		}

		OneTweakDriver::GetInstance().oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)OneTweakWndProc);
		if (config.priority.enabled) OneTweakDriver::GetInstance().SetPriority(true);
	}

	return hWnd;
}

BOOL WINAPI OneTweakDriver::HookSetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
	PrintLog(__FUNCTION__);

	if (OneTweakDriver::GetInstance().hWnd == hWnd)
	{
		auto& config = g_Host->GetConfig();
		if (config.borderless.enabled)
		{
			hWndInsertAfter = HWND_TOP;
			X = config.borderless.posX;
			Y = config.borderless.posY;
			cx = config.borderless.WindowWidth ? config.borderless.WindowWidth : cx;
			cy = config.borderless.WindowHeight ? config.borderless.WindowHeight : cy;
		}

		MH_DisableHook(MH_ALL_HOOKS);
		PrintLog("Hooks disabled");
	}

	return OneTweakDriver::GetInstance().SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

LRESULT CALLBACK OneTweakDriver::OneTweakWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto& config = g_Host->GetConfig();
	switch (uMsg)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) > 0)
		{
			if (config.priority.enabled) OneTweakDriver::GetInstance().SetPriority(true);
			if (config.double_cursor_fix.enabled) OneTweakDriver::GetInstance().ShowCursor(false);
			break;
		}
		else
		{
			if (config.priority.enabled) OneTweakDriver::GetInstance().SetPriority(false);
			if (config.double_cursor_fix.enabled) OneTweakDriver::GetInstance().ShowCursor(true);
			break;
		}
	}
	}

	return CallWindowProc(OneTweakDriver::GetInstance().oldWndProc, hWnd, uMsg, wParam, lParam);
}

void OneTweakDriver::ShowCursor(bool show)
{
	PrintLog("%s mouse cursor", show ? "Show" : "Hide");

	if (show) while (::ShowCursor(TRUE) < 0);
	else while (::ShowCursor(FALSE) > -1);
}

bool OneTweakDriver::SetPriority(bool high)
{
	auto& config = g_Host->GetConfig();

	PrintLog("Set %s priority", high ? "high" : "low");

	if (high)
		return SetPriorityClass(GetCurrentProcess(), config.priority.high) != 0;
	else
		return SetPriorityClass(GetCurrentProcess(), config.priority.low) != 0;
}

HRESULT WINAPI OneTweakDriver::HookSetCooperativeLevelA(LPDIRECTINPUT8A This, HWND hwnd, DWORD dwFlags)
{
	dwFlags &= DISCL_EXCLUSIVE;
	dwFlags |= DISCL_NONEXCLUSIVE;

	PrintLog("DirectInput set to NonExclusive mode");

	return OneTweakDriver::GetInstance().SetCooperativeLevelA(This, hwnd, dwFlags);
}

HRESULT WINAPI OneTweakDriver::HookCreateDeviceA(LPDIRECTINPUT8A This, REFGUID rguid, LPDIRECTINPUTDEVICE8A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	HRESULT hr = OneTweakDriver::GetInstance().CreateDeviceA(This, rguid, lplpDirectInputDevice, pUnkOuter);
	if (SUCCEEDED(hr) && lplpDirectInputDevice)
	{
		LPDIRECTINPUTDEVICE8A pDIDeviceA = static_cast<LPDIRECTINPUTDEVICE8A>(*lplpDirectInputDevice);
		if (pDIDeviceA->lpVtbl->SetCooperativeLevel)
		{
			void* pTarget = pDIDeviceA->lpVtbl->SetCooperativeLevel;
			MH_CreateHook(pTarget, HookSetCooperativeLevelA, reinterpret_cast<void**>(&OneTweakDriver::GetInstance().SetCooperativeLevelA));
			MH_EnableHook(pTarget);
		}
	}

	return hr;
}

HRESULT WINAPI OneTweakDriver::HookDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT hr = OneTweakDriver::GetInstance().DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	auto& config = g_Host->GetConfig();
	if (config.directinput.nonexclusive && SUCCEEDED(hr) && ppvOut)
	{
		LPDIRECTINPUT8A pDIA = static_cast<LPDIRECTINPUT8A>(*ppvOut);
		if (pDIA->lpVtbl->CreateDevice)
		{
			void* pTarget = pDIA->lpVtbl->CreateDevice;
			MH_CreateHook(pTarget, HookCreateDeviceA, reinterpret_cast<void**>(&OneTweakDriver::GetInstance().CreateDeviceA));
			MH_EnableHook(pTarget);
		}
	}

	return hr;
}
