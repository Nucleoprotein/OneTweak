#include "stdafx.h"
#include "PluginAPI_OBSE.h"

#include "OblivionHost.h"


OblivionConfig::OblivionConfig()
{

}

OblivionConfig::~OblivionConfig()
{

}
void OblivionConfig::OnReadConfig(CSimpleIniA* ini)
{
	if (!ini) return;

	PrintLog(__FUNCTION__);

	ClassName = ini->GetValue("Oblivion", "ClassName", "Oblivion");
	WindowName = ini->GetValue("Oblivion", "WindowName", "Oblivion");
}

extern "C"
{
    bool __cdecl OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
    {
        PrintLog(__FUNCTION__);
        // populate info structure
        info->infoVersion =	PluginInfo::kInfoVersion;
        info->name =		"OneTweak";
        info->version =     VERSION_REVISION;

        if(obse->isEditor)
        {
            return false;
        }
        return true;
    }

    bool __cdecl OBSEPlugin_Load(const OBSEInterface * obse)
    {
        PrintLog(__FUNCTION__);

		g_Host.reset(new OblivionHost(obse));

		return g_Host != nullptr;
    }
};