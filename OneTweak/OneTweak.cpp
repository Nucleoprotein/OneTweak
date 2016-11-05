#include "stdafx.h"
#include "OneTweak.h"
#include "Logger.h"

std::unique_ptr<OneTweakHost> g_Host(nullptr);

OneTweakHost::OneTweakHost()
{
	PrintLog("OneTweak version " VERSION_STRING);

	PrintLog(__FUNCTION__);

	SetDPIAware();
}

OneTweakHost::~OneTweakHost()
{
	PrintLog(__FUNCTION__);
}

void OneTweakHost::Run()
{
	PrintLog(__FUNCTION__);
	OneTweakDriver::GetInstance().Run();
}

void OneTweakHost::SetDPIAware()
{
	PrintLog(__FUNCTION__);

	DWORD   dwProcessSize = MAX_PATH;
	wchar_t wszProcessName[MAX_PATH];

	HANDLE hProc =
		GetCurrentProcess();

	QueryFullProcessImageNameW(hProc, 0, wszProcessName, &dwProcessSize);

	DWORD dwDisposition = 0x00;
	HKEY  key = nullptr;

	wchar_t wszKey[1024];
	lstrcpyW(wszKey, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers");

	LSTATUS status =
		RegCreateKeyExW(HKEY_CURRENT_USER,
			wszKey,
			0, NULL, 0x00L,
			KEY_READ | KEY_WRITE,
			nullptr, &key, &dwDisposition);

	if (status == ERROR_SUCCESS && key != nullptr) {
		const wchar_t* wszKillDPI = L"~ HIGHDPIAWARE";

		RegSetValueExW(key, wszProcessName, 0, REG_SZ, (BYTE *)wszKillDPI, sizeof(wchar_t) * (lstrlenW(wszKillDPI) + 1));

		RegFlushKey(key);
		RegCloseKey(key);
	}
}

