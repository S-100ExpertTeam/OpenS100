#pragma once
// Direct2D geometry for model geometries (render side).
//
// Model classes (SCurve, SSurface, ...) know nothing about Direct2D. The renderer builds
// an ID2D1Geometry on first use and parks it in the geometry's opaque RenderAttachment
// slot, so it lives and dies with the model object and the model stays platform-neutral.
// See Documents/TechDocs/2026-11-08-phase1-core-decoupling.md (step 2-1).
#include <d2d1_1.h>

class SCurve;
class SSurface;
class Scaler;

namespace GISLibrary
{
	namespace D2DGeometry
	{
		// Cached geometry in world coordinates (x, -y), created on first use.
		// The returned pointer is owned by the model object's attachment: do not Release it.
		ID2D1PathGeometry* Get(SCurve* curve, ID2D1Factory1* factory);
		ID2D1Geometry* Get(SSurface* surface, ID2D1Factory1* factory);

		// Freshly built geometry in device coordinates for the given scaler. Caller releases.
		ID2D1PathGeometry* CreateScaled(SCurve* curve, ID2D1Factory1* factory, Scaler* scaler);
		ID2D1Geometry* CreateScaled(SSurface* surface, ID2D1Factory1* factory, Scaler* scaler);
	}
}
