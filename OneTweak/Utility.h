#pragma once

#include <string>
#include <memory>
#include "Windows.h"
#include "tchar.h"

#include <Shlobj.h>
#pragma comment(lib, "shell32.lib")

#define DIR_SEP "\\"
#define DIR_SEP_CHR '\\'
#define DIR_SEP_WCHR L'\\'

inline HMODULE& CurrentModule()
{
	static HMODULE hModule = 0;
	if (!hModule)
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&hModule, &hModule);
	return hModule;
}

inline void RemoveFileSpec(char* path)
{
	if (path[strlen(path) - 4] == '.')
	{
		char* p = strrchr(path, DIR_SEP_CHR);
		if (p) *p = '\0';
	}
}

inline void RemoveFileSpec(wchar_t* path)
{
	if (path[wcslen(path) - 4] == L'.')
	{
		wchar_t* p = wcsrchr(path, DIR_SEP_WCHR);
		if (p) *p = L'\0';
	}
}

inline void RemovePath(char* path)
{
	char* p = strrchr(path, DIR_SEP_CHR);
	if (p)
	{
		strcpy_s(path, strlen(p), p + 1);
		path[strlen(p)] = '\0';
	}
}

inline void RemovePath(wchar_t* path)
{
	wchar_t* p = wcsrchr(path, DIR_SEP_WCHR);
	if (p)
	{
		wcscpy_s(path, wcslen(p), p + 1);
		path[wcslen(p)] = L'\0';
	}
}

inline void StringPathAppend(std::string* path, const std::string& more)
{
	if (path->back() != DIR_SEP_CHR)
		path->push_back(DIR_SEP_CHR);

	path->append(more);
}

inline void StringPathAppend(std::wstring* path, const std::wstring& more)
{
	if (path->back() != _T(DIR_SEP_CHR))
		path->push_back(_T(DIR_SEP_CHR));

	path->append(more);
}

inline bool FileExists(const std::string& path)
{
	FILE* filep;
	if (fopen_s(&filep, path.c_str(), "rb") == 0)
	{
		fclose(filep);
		return true;
	}
	return false;
}

inline std::string ModulePathA(HMODULE hModule)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	if (GetModuleFileNameA(hModule, buffer.get(), MAX_PATH))
	{
		std::string res = buffer.get();
		return std::move(res);
	}
	return "";
}

inline std::wstring ModulePathW(HMODULE hModule)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_PATH]);
	if (GetModuleFileNameW(hModule, buffer.get(), MAX_PATH))
	{
		std::wstring res = buffer.get();
		return std::move(res);
	}
	return L"";
}

inline std::string ModuleDirectoryA(HMODULE hModule)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	if (GetModuleFileNameA(hModule, buffer.get(), MAX_PATH))
	{
		RemoveFileSpec(buffer.get());

		std::string res = buffer.get();
		return std::move(res);
	}
	return "";
}

inline std::wstring ModuleDirectoryW(HMODULE hModule)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_PATH]);
	if (GetModuleFileNameW(hModule, buffer.get(), MAX_PATH))
	{
		RemoveFileSpec(buffer.get());

		std::wstring res = buffer.get();
		return std::move(res);
	}
	return L"";
}

inline std::string ModuleNameA(HMODULE hModule)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	if (GetModuleFileNameA(hModule, buffer.get(), MAX_PATH))
	{
		RemovePath(buffer.get());

		std::string res = buffer.get();
		return std::move(res);
	}
	return "";
}

inline std::wstring ModuleNameW(HMODULE hModule)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_PATH]);
	if (GetModuleFileNameW(hModule, buffer.get(), MAX_PATH))
	{
		RemovePath(buffer.get());

		std::wstring res = buffer.get();
		return std::move(res);
	}
	return L"";
}

inline std::string CreateSystemModulePath(const char* module_name)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	GetSystemDirectoryA(buffer.get(), MAX_PATH);

	std::string path(buffer.get());
	StringPathAppend(&path, module_name);

	return std::move(path);
}

inline std::wstring CreateSystemModulePath(const wchar_t* module_name)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_PATH]);
	GetSystemDirectoryW(buffer.get(), MAX_PATH);

	std::wstring path(buffer.get());
	StringPathAppend(&path, module_name);

	return std::move(path);
}

inline HMODULE LoadLibraryFromDirectory(const char* dir, const char* module_name)
{
	std::string path(dir);
	StringPathAppend(&path, module_name);
	return LoadLibraryA(path.c_str());
}

inline HMODULE LoadLibraryFromSystemDir(const char* module_name)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	GetSystemDirectoryA(buffer.get(), MAX_PATH);
	return LoadLibraryFromDirectory(buffer.get(), module_name);
}

inline std::string FullPathFromPath(const std::string& path)
{
	// check it already full path
	if (path[1] == ':' || path[1] == '?')
		return path;

	std::string out_path = ModuleDirectoryA(CurrentModule());

	if (out_path.back() != DIR_SEP_CHR && path.front() != DIR_SEP_CHR)
		out_path.push_back(DIR_SEP_CHR);

	std::string res = out_path + path;
	return std::move(res);
}


