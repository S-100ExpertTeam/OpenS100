#pragma once
// Platform headers for the Direct2D render module (S100Engine).
//
// Every S100Engine header that names a Direct2D / DirectWrite / WIC / Win32 type includes
// this file so it is self-contained and does not rely on the including project's
// precompiled header. Model projects (PortrayalCatalogue, FeatureCatalog, ...) must not
// include it. Phase 1 step 3-4 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
// No NOMINMAX / WIN32_LEAN_AND_MEAN here: GDI+ (used by the background layer) needs the
// min/max macros and the full Win32 surface.
#include <windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef D2_SAFE_RELEASE
#define D2_SAFE_RELEASE
template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}
#endif
