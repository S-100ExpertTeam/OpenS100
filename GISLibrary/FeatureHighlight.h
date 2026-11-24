#pragma once
// What a display draws over a chart to mark things: the feature that was picked, the vertices
// of a feature being edited, a box with the name of a layer. On any backend, in device pixels.
#include "IRenderer.h"

#include <string>

class SGeometry;
class Scaler;

namespace GISLibrary
{
	// The geometry of a picked feature in red: corner brackets about a point, a ring about
	// every point of a multipoint, a thick line along a curve, a translucent fill of a
	// surface. Moved by an offset in pixels (the view while it is being dragged).
	void DrawFeatureHighlight(s100::render::IRenderer& renderer, Scaler& scaler, SGeometry* geometry, float offsetX, float offsetY);

	// A vertex of a feature being edited: a white square, with a red edge when it is the one
	// selected.
	void DrawVertexHandle(s100::render::IRenderer& renderer, float x, float y, bool selected);

	// A translucent red box with two lines of text at its top left (a layer that cannot be drawn).
	void DrawInformationBox(s100::render::IRenderer& renderer, float left, float top, float right, float bottom,
		const std::string& firstLine, const std::string& secondLine);
}
