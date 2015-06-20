#pragma once

#include "OneTweakConfig.h"

class OneTweakDriver : NonCopyable
{
public:
	OneTweakDriver();
	~OneTweakDriver();

	static OneTweakDriver& GetInstance()
	{
		static OneTweakDriver instance;
		return instance;
	}

	void Run();

private:
	HWND hWnd;
	WNDPROC oldWndProc;
	static LRESULT CALLBACK OneTweakWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	UINT(WINAPI* GetPrivateProfileIntA)(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);
	HWND(WINAPI* CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	BOOL(WINAPI* SetWindowPos)(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

	static UINT WINAPI HookGetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);
	static HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	static BOOL WINAPI HookSetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

	void ShowCursor(bool show);
	bool SetPriority(bool high);

};

