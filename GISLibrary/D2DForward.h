#pragma once
// Forward declarations of the Direct2D types the drawing-instruction classes (SENC_*) still take
// in their draw functions. The headers only pass pointers along, so they need no <d2d1.h>; the
// sources that draw include the Direct2D headers themselves.
//
// Goes away when drawing leaves the instruction classes (Phase 3, render backend).
struct ID2D1DCRenderTarget;
struct ID2D1Factory1;
struct ID2D1PathGeometry;
struct ID2D1SolidColorBrush;
struct ID2D1StrokeStyle1;
struct DWRITE_TEXT_METRICS;

class SVGReader;

namespace GISLibrary
{
	class D2D1Resources;
}
