#pragma once

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
#include <tchar.h>
#include <comdef.h>
#include <atlbase.h>
#include <atlconv.h>
#include <atlsafe.h>

#include "Common.h"
#include <gdiplus.h>
// GDI+ names were previously pulled in unqualified through a PortrayalCatalogue header; keep
// that for the legacy GDI drawing code in this project until it moves to the renderer.
using namespace Gdiplus;

#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "gdiplus.lib")
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
