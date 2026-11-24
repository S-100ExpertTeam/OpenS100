#pragma once
// Plain value types of the drawing-instruction model (SENC_*) and of the renderer interface, so
// that neither names the types of a graphics library.
namespace GISLibrary
{
	// Straight (not premultiplied) RGBA, each component 0..1. Same layout as D2D1_COLOR_F.
	struct ColorF
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;
	};

	// A point in the float coordinates the renderer draws in. Same layout as D2D1_POINT_2F.
	struct PointF
	{
		float x = 0.0f;
		float y = 0.0f;
	};
}
