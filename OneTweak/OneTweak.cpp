#include "stdafx.h"
#include "OneTweak.h"
#include "Logger.h"

std::unique_ptr<OneTweakHost> g_Host(nullptr);

OneTweakHost::OneTweakHost()
{
	PrintLog("OneTweak version "VERSION_STRING);

	PrintFunc;

	this->SetProcessDPIAware();
}

OneTweakHost::~OneTweakHost()
{
	PrintFunc;
}

void OneTweakHost::Run()
{
	PrintFunc;
    OneTweakDriver::GetInstance().Run();
}

void OneTweakHost::SetProcessDPIAware()
{
	PrintFunc;

	typedef BOOL(WINAPI* SetProcessDPIAware_t)(void);
	HMODULE user32_dll = GetModuleHandle("user32.dll");
	SetProcessDPIAware_t dpiaware = (SetProcessDPIAware_t)GetProcAddress(user32_dll, "SetProcessDPIAware");
	if (dpiaware) dpiaware();
}

