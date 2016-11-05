#pragma once

#include <SimpleIni.h>

#define CONFIG_VERSION 11

class OneTweakConfig
{
public:
    OneTweakConfig();
    virtual ~OneTweakConfig();

    void ReadConfig();

    virtual void OnSaveConfig(CSimpleIniA* ini) {};
	virtual void OnReadConfig(CSimpleIniA* ini) {};

    DWORD version;

    std::string ClassName;
    std::string WindowName;

    struct
    {
        bool enabled;

        bool active;

        DWORD flags;
        DWORD flagsEx;

        int posX;
        int posY;

        DWORD WindowWidth;
        DWORD WindowHeight;

        DWORD RenderWidth;
        DWORD RenderHeight;

    } borderless;

    struct
    {
        bool enabled;
        DWORD high;
        DWORD low;
    } priority;

    struct
    {
        bool enabled;
    } double_cursor_fix;

	struct
	{
		bool nonexclusive;
	} directinput;

protected:
	void SaveConfig(CSimpleIniA* ini);
	std::string inifilename;

private:
    DWORD PriorityIdToClass(DWORD id);
};

