#pragma once
// Legacy GDI drawing of GeoMetryLibrary shapes (render side).
//
// GeoMetryLibrary is pure geometry: it no longer knows HDC or POINT. The shapefile
// background layer and the record-level debug draw use this renderer instead.
// Phase 1 step 3-4 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <windows.h>

class Geometry;
class Scaler;

namespace GISLibrary
{
	namespace GdiRender
	{
		// Draws a GeoPoint/GeoPointZ/GeoMultiPoint/GeoMultiPointZ/GeoPolyline/GeoPolygon
		// with plain GDI. Other geometry types are ignored.
		void Draw(Geometry& geometry, HDC& hDC, Scaler* scaler, double offset = 0.0);
	}
}
