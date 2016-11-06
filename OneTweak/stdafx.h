// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define STRICT
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <shlobj.h>

#include <d3d9.h>

#include <string>
#include <memory>
#include <algorithm>
#include <vector>

// TODO: reference additional headers your program requires here
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Types.h"
#include "version.h"
#include "IDebugLog.h"


