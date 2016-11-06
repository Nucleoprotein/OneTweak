#include "stdafx.h"
#include "PluginAPI_SKSE.h"
#include "skse_version.h"

#include "SkyrimHost.h"
#include "SafeWrite.h"

SkyrimConfig::SkyrimConfig() :
precachekiller(false),
nointro(false)
{
	_MESSAGE(__FUNCTION__);
}

SkyrimConfig::~SkyrimConfig()
{
	_MESSAGE(__FUNCTION__);
}

void SkyrimConfig::OnReadConfig(CSimpleIniA* ini)
{
	if (!ini) return;

	_MESSAGE(__FUNCTION__);

	ClassName = ini->GetValue("Skyrim", "ClassName", "Skyrim");
	WindowName = ini->GetValue("Skyrim", "WindowName", "Skyrim");

	precachekiller = ini->GetBoolValue("Skyrim", "PrecacheKiller");
	nointro = ini->GetBoolValue("Skyrim", "NoIntro");
}

void SkyrimConfig::OnSaveConfig(CSimpleIniA* ini)
{
	if (!ini) return;

	_MESSAGE(__FUNCTION__);

    ini->SetBoolValue("Skyrim", "PrecacheKiller", true, "#Enable ShowRaceMenu Precache Killer");
	ini->SetBoolValue("Skyrim", "NoIntro", true, "#Disable intro video");
}

void SkyrimHost::PrecacheKiller()
{
	_MESSAGE(__FUNCTION__);

	SafeWrite8(0x008868C0, 0xC3); _MESSAGE("PrecacheCharGen disabled");
	SafeWrite8(0x00886B50, 0xC3); _MESSAGE("PrecacheCharGenClear disabled");
}

void SkyrimHost::NoIntro()
{
	//off: 00CCBD60
	//va:  010CD560
	//rva: 00CCD560

	SafeWrite8(0x010CD560, 0x00); _MESSAGE("Intro disabled");
}

extern "C"
{
	bool __cdecl SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim\\SKSE\\skse_OneTweak.log");

		_MESSAGE(__FUNCTION__);
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
		_MESSAGE(__FUNCTION__);

		g_Host.reset(new SkyrimHost(skse));

		return g_Host ? g_Host->Run() : 0;
	}
};