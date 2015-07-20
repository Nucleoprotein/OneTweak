#pragma once

#include <string>
#include "Windows.h"
#include "CommonTypes.h"

inline HMODULE& CurrentModule()
{
    static HMODULE hModule = 0;
    if (!hModule)
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&hModule, &hModule);
    return hModule;
}

bool FileExist(const std::string& path);
std::string FullPathFromPath(const std::string& path);

std::string ModulePathA(std::string* out, HMODULE hModule);
std::wstring ModulePathW(HMODULE hModule);
std::string ModuleDirectoryA(HMODULE hModule);
std::wstring ModuleDirectoryW(HMODULE hModule);
std::string ModuleFileNameA(HMODULE hModule);
std::wstring ModuleFileNameW(HMODULE hModule);

void StringToGUID(GUID* id, const std::string& szBuf);
void StringToGUID(GUID* id, const std::wstring& szBuf);

bool GUIDtoString(std::string* out, const GUID &g);
bool GUIDtoString(std::wstring* out, const GUID &g);





