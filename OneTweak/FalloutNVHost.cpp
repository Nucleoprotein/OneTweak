#include "stdafx.h"
#include "PluginAPI_NVSE.h"

#include "FalloutNVHost.h"

FalloutNVConfig::FalloutNVConfig()
{

}

FalloutNVConfig::~FalloutNVConfig()
{

}

void FalloutNVConfig::OnReadConfig(IniFile* ini)
{
	if (!ini) return;

	PrintLog(__FUNCTION__);

    ini->Get("FalloutNV", "ClassName", &ClassName, "Fallout: New Vegas");
    ini->Get("FalloutNV", "WindowName", &WindowName, "Fallout: New Vegas");
}

extern "C"
{
    bool __cdecl NVSEPlugin_Query(const NVSEInterface * nvse, PluginInfo * info)
    {
        PrintLog(__FUNCTION__);
        // populate info structure
        info->infoVersion =	PluginInfo::kInfoVersion;
        info->name =		"OneTweak";
        info->version =     VERSION_REVISION;

        if(nvse->isEditor)
        {
            return false;
        }
        return true;
    }

    bool __cdecl NVSEPlugin_Load(const NVSEInterface * nvse)
    {
        PrintLog(__FUNCTION__);

		g_Host.reset(new FalloutNVHost(nvse));

		return g_Host != nullptr;
    }
};