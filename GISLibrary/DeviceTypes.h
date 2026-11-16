#pragma once
// Device-space types named by the legacy GDI entry points and view-point buffers that the
// geometry and dataset classes still carry.
//
// On Windows these are the Win32 types themselves, forward-declared so that a model header
// does not need <windows.h>; a source that uses their members includes <windows.h> (the
// precompiled header does). Elsewhere POINT and RECT are plain structs of longs and HDC stays opaque:
// the GDI drawing code is not built there.
//
// Goes away when drawing leaves the model classes (Phase 3, render backend).
#ifdef _WIN32
struct tagPOINT;
typedef tagPOINT POINT;
struct tagRECT;
typedef tagRECT RECT;
#else
struct POINT
{
	long x;
	long y;
};
struct RECT
{
	long left;
	long top;
	long right;
	long bottom;
};
#endif

struct HDC__;
typedef HDC__* HDC;
