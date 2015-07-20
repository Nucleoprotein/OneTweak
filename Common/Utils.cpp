#include "stdafx.h"

#include "Common.h"
#include <string>
#include <memory>

#include <Shlobj.h>
#pragma comment(lib, "shell32.lib")

void RemoveFileSpec(char* path)
{
	if (path[strlen(path) - 4] == '.')
	{
		char* p = strrchr(path, '\\');
		if (p) *p = '\0';
	}
}

void RemoveFileSpec(wchar_t* path)
{
	if (path[wcslen(path) - 4] == L'.')
	{
		wchar_t* p = wcsrchr(path, L'\\');
		if (p) *p = L'\0';
	}
}

bool FileExist(const std::string& path)
{
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}
	return false;
}

std::string FullPathFromPath(const std::string& path)
{
	// check it already full path
	if (path[1] == ':' || path[1] == '?')
		return path;

	std::string out_path = ModuleDirectoryA(CurrentModule());

	if (out_path.back() != '\\' && path.front() != '\\')
		out_path.push_back('\\');

	return out_path + path;
}

std::string ModulePathA(std::string* out, HMODULE hModule)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	GetModuleFileNameA(hModule, buffer.get(), MAX_PATH);
	return buffer.get();
}

std::wstring ModulePathW(HMODULE hModule)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_PATH]);
	GetModuleFileNameW(hModule, buffer.get(), MAX_PATH);
	return buffer.get();
}

std::string ModuleDirectoryA(HMODULE hModule)
{
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
	GetModuleFileNameA(hModule, buffer.get(), MAX_PATH);
	RemoveFileSpec(buffer.get());
	return buffer.get();
}

std::wstring ModuleDirectoryW(HMODULE hModule)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_PATH]);
	GetModuleFileNameW(hModule, buffer.get(), MAX_PATH);
	RemoveFileSpec(buffer.get());
	return buffer.get();
}

void StringToGUID(GUID* id, const std::string& szBuf)
{
	const char* p = szBuf.c_str();
	if (strchr(p, '{')) p++;

	u32 d1;
	s32 d2, d3;
	s32 b[8];

	if (sscanf_s(p, "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		&d1, &d2, &d3, &b[0], &b[1], &b[2], &b[3], &b[4], &b[5], &b[6], &b[7]) != 11)
	{
		*id = GUID_NULL;
		return;
	}

	id->Data1 = d1;
	id->Data2 = (u16)d2;
	id->Data3 = (u16)d3;

	for (int i = 0; i < 8; ++i)
		id->Data4[i] = (u8)b[i];

	return;
}

void StringToGUID(GUID* id, const std::wstring& szBuf)
{
	const wchar_t* p = szBuf.c_str();
	if (wcschr(p, L'{')) p++;

	u32 d1;
	s32 d2, d3;
	s32 b[8];

	if (swscanf_s(p, L"%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		&d1, &d2, &d3, &b[0], &b[1], &b[2], &b[3], &b[4], &b[5], &b[6], &b[7]) != 11)
	{
		*id = GUID_NULL;
		return;
	}

	id->Data1 = d1;
	id->Data2 = (u16)d2;
	id->Data3 = (u16)d3;

	for (int i = 0; i < 8; ++i)
		id->Data4[i] = (u8)b[i];

	return;
}

bool GUIDtoString(std::string* out, const GUID &g)
{
	std::unique_ptr<char[]> buffer(new char[40]);
	sprintf_s(buffer.get(), 40, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		g.Data1, g.Data2, g.Data3, g.Data4[0], g.Data4[1], g.Data4[2], g.Data4[3], g.Data4[4], g.Data4[5], g.Data4[6], g.Data4[7]);

	*out = buffer.get();
	return !out->empty();
}

bool GUIDtoString(std::wstring* out, const GUID &g)
{
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[40]);
	swprintf_s(buffer.get(), 40, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		g.Data1, g.Data2, g.Data3, g.Data4[0], g.Data4[1], g.Data4[2], g.Data4[3], g.Data4[4], g.Data4[5], g.Data4[6], g.Data4[7]);

	*out = buffer.get();
	return !out->empty();
}


