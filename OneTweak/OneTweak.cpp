#include "stdafx.h"
#include "OneTweak.h"
std::unique_ptr<OneTweakHost> g_Host(nullptr);

OneTweakHost::OneTweakHost()
{
	_MESSAGE("OneTweak version " VERSION_STRING);

	_MESSAGE(__FUNCTION__);

	SetDPIAware();
}

OneTweakHost::~OneTweakHost()
{
	_MESSAGE(__FUNCTION__);
}

bool OneTweakHost::Run()
{
	_MESSAGE(__FUNCTION__);
	return OneTweakDriver::GetInstance().Run();
}

void OneTweakHost::SetDPIAware()
{
	_MESSAGE(__FUNCTION__);

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

