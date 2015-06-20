#pragma once

#include <shlwapi.h>

#define CONFIG_VERSION 7

class OneTweakConfig
{
public:
    OneTweakConfig();
    virtual ~OneTweakConfig();

    void ReadConfig();

    virtual void OnSaveConfig(IniFile* ini) {};
    virtual void OnReadConfig(IniFile* ini) {};

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

protected:
    void SaveConfig(IniFile* ini);

private:
    DWORD PriorityIdToClass(DWORD id);
};

