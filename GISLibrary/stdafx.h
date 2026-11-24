#pragma once

// Standard headers the sources of this project rely on through the precompiled header.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#ifdef _WIN32
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN           
#endif

#include "targetver.h"

// GISLibrary builds without MFC (Phase 1 step 3-5a). Win32 + COM + ATL conversion helpers
// remain while the Direct2D renderer lives in this project; they go with it in Phase 3.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // keep commdlg.h and friends out of the PCH (their function-name macros clash with model methods)
#endif
#include <windows.h>
// GISLIBRARY_MODEL_ONLY: the dataset model and readers built on their own (CMake target
// opens100::dataset). They need none of the COM, ATL, GDI+ and Direct2D headers below.
#ifndef GISLIBRARY_MODEL_ONLY
#include <tchar.h>
#include <comdef.h>
#include <atlbase.h>
#include <atlconv.h>
#include <atlsafe.h>
#endif // GISLIBRARY_MODEL_ONLY
#endif // _WIN32

#include "Common.h"

#if defined(_WIN32) && !defined(GISLIBRARY_MODEL_ONLY)
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "winmm.lib")

#ifndef D2_SAFE_RELEASE
#define D2_SAFE_RELEASE

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#endif
#endif // _WIN32 && !GISLIBRARY_MODEL_ONLY
