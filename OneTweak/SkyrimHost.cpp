#include "stdafx.h"
#include "PluginAPI_SKSE.h"
#include "skse_version.h"

#include "SkyrimHost.h"
#include "SafeWrite.h"

SkyrimConfig::SkyrimConfig() :
precachekiller(false),
nointro(false)
{
	PrintLog(__FUNCTION__);
}

SkyrimConfig::~SkyrimConfig()
{
	PrintLog(__FUNCTION__);
}

void SkyrimConfig::OnReadConfig(IniFile* ini)
{
	if (!ini) return;

	PrintLog(__FUNCTION__);

    ini->Get("Skyrim", "ClassName", &ClassName, "Skyrim");
    ini->Get("Skyrim", "WindowName", &WindowName, "Skyrim");

    ini->Get("Skyrim", "PrecacheKiller", &precachekiller);
    ini->Get("Skyrim", "NoIntro", &nointro);
}

void SkyrimConfig::OnSaveConfig(IniFile* ini)
{
	if (!ini) return;

	PrintLog(__FUNCTION__);

    ini->Set("Skyrim", "PrecacheKiller", true, "#Enable ShowRaceMenu Precache Killer");
    ini->Set("Skyrim", "NoIntro", true, "#Disable intro video");
}

void SkyrimHost::PrecacheKiller()
{
	PrintLog(__FUNCTION__);

	SafeWrite8(0x008868C0, 0xC3); PrintLog("PrecacheCharGen disabled");
	SafeWrite8(0x00886B50, 0xC3); PrintLog("PrecacheCharGenClear disabled");
}

void SkyrimHost::NoIntro()
{
	//off: 00CCBD60
	//va:  010CD560
	//rva: 00CCD560

	SafeWrite8(0x010CD560, 0x00); PrintLog("Intro disabled");
}

extern "C"
{
	bool __cdecl SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		PrintLog(__FUNCTION__);
		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "OneTweak";
		info->version = VERSION_REVISION;

		if (skse->isEditor != 0)
		{
			return false;
		}
        
        if (skse->skseVersion < MAKE_SKYRIM_VERSION(1, 7, 0))
        {
            MessageBoxA(NULL, "You using too old SKSE, please upgrade to 1.7.0 or later.", "OneTweak", MB_ICONINFORMATION);
            return false;
        }

		return true;
	}

	bool __cdecl SKSEPlugin_Load(const SKSEInterface * skse)
	{
		PrintLog(__FUNCTION__);

		g_Host.reset(new SkyrimHost(skse));

		return g_Host != nullptr;
	}
};