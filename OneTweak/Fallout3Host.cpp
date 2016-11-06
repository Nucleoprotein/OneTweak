#include "stdafx.h"
#include "PluginAPI_FOSE.h"

#include "Fallout3Host.h"

Fallout3Config::Fallout3Config()
{

}

Fallout3Config::~Fallout3Config()
{

}

void Fallout3Config::OnReadConfig(CSimpleIniA* ini)
{
	if (!ini) return;

	_MESSAGE(__FUNCTION__);

	ClassName = ini->GetValue("Fallout3", "ClassName", "Fallout3");
	WindowName = ini->GetValue("Fallout3", "WindowName", "Fallout3");
}

extern "C"
{
    bool __cdecl FOSEPlugin_Query(const FOSEInterface * fose, PluginInfo * info)
    {
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout3\\FOSE\\fose_OneTweak.log");

        _MESSAGE(__FUNCTION__);
        // populate info structure
        info->infoVersion =	PluginInfo::kInfoVersion;
        info->name =		"OneTweak";
        info->version =     VERSION_REVISION;

        if(fose->isEditor)
        {
            return false;
        }
        return true;
    }

    bool __cdecl FOSEPlugin_Load(const FOSEInterface * fose)
    {
        _MESSAGE(__FUNCTION__);

		g_Host.reset(new Fallout3Host(fose));

		return g_Host ? g_Host->Run() : 0;
    }
};