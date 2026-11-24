#pragma once
// Drawing of GeoMetryLibrary shapes (a shapefile: the background map of the world, a layer that
// is no S-100 dataset) on a renderer.
//
// GeoMetryLibrary is pure geometry; this is the render side of it.
#include "IRenderer.h"

class Geometry;
class Scaler;

namespace GISLibrary
{
	// Draws a GeoPoint/GeoPointZ/GeoMultiPoint/GeoMultiPointZ/GeoPolyline/GeoPolygon: a polygon
	// white with a black edge, a line black, points as small squares, the height of a point as
	// a number. Other geometry types are ignored. `offset` moves the shape along x (the copy of
	// the world a turn of longitude away).
	void DrawGeometry(s100::render::IRenderer& renderer, Geometry& geometry, Scaler& scaler, double offset = 0.0);
}
