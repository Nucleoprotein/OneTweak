#include "stdafx.h"
#include "OneTweakConfig.h"
#include "Utility.h"

OneTweakConfig::OneTweakConfig() :
version(0)
{
}

void OneTweakConfig::SaveConfig(CSimpleIniA* ini)
{
	_MESSAGE(__FUNCTION__);

	ini->Reset();
	version = CONFIG_VERSION;
	ini->SetLongValue("Version", "Config", version, "#Configuration file version, DO NOT CHANGE");

	ini->SetBoolValue("Global", "BorderlessWindow", true, "#Enable Borderless Window");
	ini->SetBoolValue("Global", "DoubleCursorFix", true, "#Enable Double Cursor Fix");
	ini->SetBoolValue("Global", "Priority", false, "#Enable dynamic priority change");

	ini->SetLongValue("Priority", "Foreground", 3, "#0 - idle, 1 - below normal, 2 - normal, 3 - above normal, 4 - high, 5 - realtime");
	ini->SetLongValue("Priority", "Background", 1, "#0 - idle, 1 - below normal, 2 - normal, 3 - above normal, 4 - high, 5 - realtime");

	ini->SetBoolValue("BorderlessWindow", "Active", true, "#Active in background");

	ini->SetLongValue("BorderlessWindow", "StyleFlags", 0, "#Window style flags");
	ini->SetLongValue("BorderlessWindow", "StyleFlagsEx", 0, "#Window style flags ex");

	ini->SetLongValue("BorderlessWindow", "WindowPosX", 0, "#Window X position");
	ini->SetLongValue("BorderlessWindow", "WindowPosY", 0, "#Window Y position");

	ini->SetLongValue("BorderlessWindow", "WindowWidth", 0, "#Window width, 0 = autodetect");
	ini->SetLongValue("BorderlessWindow", "WindowHeight", 0, "#Window height, 0 = autodetect");

	ini->SetLongValue("BorderlessWindow", "RenderWidth", 0, "#Rendering width, 0 = autodetect");
	ini->SetLongValue("BorderlessWindow", "RenderHeight", 0, "#Rendering height, 0 = autodetect");

	ini->SetBoolValue("DirectInput", "NonExclusive", true, "#Allows Alt+F4 and multimedia keys");

	OnSaveConfig(ini);
	ini->SaveFile(inifilename.c_str());
}

void OneTweakConfig::ReadConfig()
{
	_MESSAGE(__FUNCTION__);

	inifilename = FullPathFromPath("OneTweak.ini");

	CSimpleIniA ini;
	ini.LoadFile(inifilename.c_str());

	version = ini.GetLongValue("Version", "Config");
	if (version != CONFIG_VERSION)
	{
		SaveConfig(&ini);
	}

	borderless.enabled = ini.GetBoolValue("Global", "BorderlessWindow");
	double_cursor_fix.enabled = ini.GetBoolValue("Global", "DoubleCursorFix");

	priority.enabled = ini.GetBoolValue("Global", "Priority");

	if (priority.enabled)
	{
		DWORD id_fg = ini.GetLongValue("Priority", "Foreground");
		DWORD id_bg = ini.GetLongValue("Priority", "Background");

		priority.high = PriorityIdToClass(id_fg);
		priority.low = PriorityIdToClass(id_bg);
	}

	if (borderless.enabled)
	{
		borderless.active = ini.GetBoolValue("BorderlessWindow", "Active");

		borderless.flags = ini.GetLongValue("BorderlessWindow", "StyleFlags");
		borderless.flagsEx = ini.GetLongValue("BorderlessWindow", "StyleFlagsEx");

		borderless.posX = ini.GetLongValue("BorderlessWindow", "WindowPosX");
		borderless.posY = ini.GetLongValue("BorderlessWindow", "WindowPosY");

		borderless.WindowWidth = ini.GetLongValue("BorderlessWindow", "WindowWidth");
		borderless.WindowHeight = ini.GetLongValue("BorderlessWindow", "WindowHeight");

		borderless.RenderWidth = ini.GetLongValue("BorderlessWindow", "RenderWidth");
		borderless.RenderHeight = ini.GetLongValue("BorderlessWindow", "RenderHeight");
	}

	directinput.nonexclusive = ini.GetBoolValue("DirectInput", "NonExclusive");

	OnReadConfig(&ini);
}

OneTweakConfig::~OneTweakConfig()
{
	_MESSAGE(__FUNCTION__);
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