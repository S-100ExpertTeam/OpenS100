#pragma once
// The device context of Windows, by name only: the Windows end of the view (SkiaViewFrame,
// CGISLibraryApp::Draw on a device context) names it, and no header of the library needs
// <windows.h> for it. The points and rectangles of the device are the library's own
// (GeoMetryLibrary/ScreenTypes.h).
#include "../GeoMetryLibrary/ScreenTypes.h"

struct HDC__;
typedef HDC__* HDC;
