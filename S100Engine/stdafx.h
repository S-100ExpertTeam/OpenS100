#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN           
#endif

#include "targetver.h"

// S100Engine builds without MFC (Phase 1 step 3-5b). Direct2D/DirectWrite/WIC stay until the
// Skia renderer replaces this module (Phase 3).
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <tchar.h>

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#define FACTOR 0.1f
#define PIXEL_MM 0.161f
#define WIDTH_MM 360.0 * FACTOR * PIXEL_MM
#define EARTH_2PIR 40075000000.0f
#define SCALE_OFFSET 10

#define IsCTRLPressed()  ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000 ))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define LAT_LIMIT 85.0f
float ProjectionX(float lon);
float ProjectionY(float lat);
D2D1_POINT_2F Projection(D2D1_POINT_2F point);
D2D1_POINT_2F ProjectionAndApplyFactor(D2D1_POINT_2F point);
float InverseProjectionX(float lon);
float InverseProjectionY(float lat);
D2D1_POINT_2F InverseProjection(D2D1_POINT_2F point);

#include "../GeoMetryLibrary/TextCodec.h"

#define OUTPUT_DEBUG_STRING 1

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