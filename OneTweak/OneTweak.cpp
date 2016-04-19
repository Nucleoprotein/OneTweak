#include "stdafx.h"
#include "OneTweak.h"
#include "Logger.h"

std::unique_ptr<OneTweakHost> g_Host(nullptr);

OneTweakHost::OneTweakHost()
{
	PrintLog("OneTweak version " VERSION_STRING);

	PrintLog(__FUNCTION__);

	this->SetProcessDPIAware();
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

void OneTweakHost::SetProcessDPIAware()
{
	PrintLog(__FUNCTION__);

	typedef BOOL(WINAPI* SetProcessDPIAware_t)(void);
	HMODULE user32_dll = GetModuleHandle("user32.dll");
	SetProcessDPIAware_t dpiaware = (SetProcessDPIAware_t)GetProcAddress(user32_dll, "SetProcessDPIAware");
	if (dpiaware) dpiaware();
}

