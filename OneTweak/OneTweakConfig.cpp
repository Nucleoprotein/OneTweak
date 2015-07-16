#include "stdafx.h"
#include "OneTweakConfig.h"


OneTweakConfig::OneTweakConfig() :
version(0)
{
}

void OneTweakConfig::SaveConfig(IniFile* ini)
{
    PrintFunc;

    version = CONFIG_VERSION;
    ini->Set("Version", "Config", version, "#Configuration file version, DO NOT CHANGE");

    ini->Set("Global", "BorderlessWindow", true, "#Enable Borderless Window");
    ini->Set("Global", "DoubleCursorFix", true, "#Enable Double Cursor Fix");
    ini->Set("Global", "Priority", false, "#Enable dynamic priority change");

    ini->Set("Priority", "Foreground", 3, "#0 - idle, 1 - below normal, 2 - normal, 3 - above normal, 4 - high, 5 - realtime");
    ini->Set("Priority", "Background", 1, "#0 - idle, 1 - below normal, 2 - normal, 3 - above normal, 4 - high, 5 - realtime");

    ini->Set("BorderlessWindow", "Active", true, "#Active in background");

    ini->Set("BorderlessWindow", "StyleFlags", 0, "#Window style flags");
    ini->Set("BorderlessWindow", "StyleFlagsEx", 0, "#Window style flags ex");

    ini->Set("BorderlessWindow", "WindowPosX", 0, "#Window X position");
    ini->Set("BorderlessWindow", "WindowPosY", 0, "#Window Y position");

    ini->Set("BorderlessWindow", "WindowWidth", 0, "#Window width, 0 = autodetect");
    ini->Set("BorderlessWindow", "WindowHeight", 0, "#Window height, 0 = autodetect");

    ini->Set("BorderlessWindow", "RenderWidth", 0, "#Rendering width, 0 = autodetect");
    ini->Set("BorderlessWindow", "RenderHeight", 0, "#Rendering height, 0 = autodetect");

    OnSaveConfig(ini);
    ini->Save();
}

void OneTweakConfig::ReadConfig()
{
    PrintFunc;

    IniFile ini;
    ini.Load("OneTweak.ini");

    if (!ini.Get("Version", "Config", &version) || version != CONFIG_VERSION)
    {
        SaveConfig(&ini);
    }

    ini.Get("Global", "BorderlessWindow", &borderless.enabled);
    ini.Get("Global", "DoubleCursorFix", &double_cursor_fix.enabled);

    ini.Get("Global", "Priority", &priority.enabled);

    if (priority.enabled)
    {
        DWORD id_fg;
        DWORD id_bg;

        ini.Get("Priority", "Foreground", &id_fg);
        ini.Get("Priority", "Background", &id_bg);

        priority.high = PriorityIdToClass(id_fg);
        priority.low = PriorityIdToClass(id_bg);
    }

    if (borderless.enabled)
    {
        ini.Get("BorderlessWindow", "Active", &borderless.active);

        ini.Get("BorderlessWindow", "StyleFlags", &borderless.flags);
        ini.Get("BorderlessWindow", "StyleFlagsEx", &borderless.flagsEx);

        ini.Get("BorderlessWindow", "WindowPosX", &borderless.posX);
        ini.Get("BorderlessWindow", "WindowPosY", &borderless.posY);

        ini.Get("BorderlessWindow", "WindowWidth", &borderless.WindowWidth);
        ini.Get("BorderlessWindow", "WindowHeight", &borderless.WindowHeight);

        ini.Get("BorderlessWindow", "RenderWidth", &borderless.RenderWidth);
        ini.Get("BorderlessWindow", "RenderHeight", &borderless.RenderHeight);
    }

    OnReadConfig(&ini);
}

OneTweakConfig::~OneTweakConfig()
{
    PrintFunc;
}

DWORD OneTweakConfig::PriorityIdToClass(DWORD id)
{
    if (id > 5) id = 5;

    const DWORD priorities[] =
    {
        IDLE_PRIORITY_CLASS,
        BELOW_NORMAL_PRIORITY_CLASS,
        NORMAL_PRIORITY_CLASS,
        ABOVE_NORMAL_PRIORITY_CLASS,
        HIGH_PRIORITY_CLASS,
        REALTIME_PRIORITY_CLASS,
    };

    return priorities[id];
}