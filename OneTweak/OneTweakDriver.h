#pragma once

#include "OneTweakConfig.h"

class OneTweakDriver
{
public:
	OneTweakDriver();
	~OneTweakDriver();

	OneTweakDriver(const OneTweakDriver&&) = delete;
	void operator=(const OneTweakDriver&&) = delete;
	OneTweakDriver(OneTweakDriver&) = delete;
	OneTweakDriver& operator=(OneTweakDriver& other) = delete;

	static OneTweakDriver& GetInstance()
	{
		static OneTweakDriver instance;
		return instance;
	}

	bool Run();

private:
	HWND hWnd;
	WNDPROC oldWndProc;
	static LRESULT CALLBACK OneTweakWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	UINT(WINAPI* GetPrivateProfileIntA)(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);
	HWND(WINAPI* CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	BOOL(WINAPI* SetWindowPos)(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	HRESULT(WINAPI* DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);
	HRESULT(WINAPI* CreateDeviceA)(LPDIRECTINPUT8A This, REFGUID rguid, LPDIRECTINPUTDEVICE8A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
	HRESULT(WINAPI* SetCooperativeLevelA)(LPDIRECTINPUT8A This, HWND hwnd, DWORD dwFlags);

	static UINT WINAPI HookGetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);
	static HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	static BOOL WINAPI HookSetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	static HRESULT WINAPI HookDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);
	static HRESULT WINAPI HookCreateDeviceA(LPDIRECTINPUT8A This, REFGUID rguid, LPDIRECTINPUTDEVICE8A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
	static HRESULT WINAPI HookSetCooperativeLevelA(LPDIRECTINPUT8A This, HWND hwnd, DWORD dwFlags);

	void ShowCursor(bool show);
	bool SetPriority(bool high);
};

