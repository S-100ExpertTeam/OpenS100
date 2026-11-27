#pragma once
// Plain value types of the drawing-instruction model (SENC_*) and of the renderer interface, so
// that neither names the types of a graphics library.
namespace GISLibrary
{
	// Straight (not premultiplied) RGBA, each component 0..1.
	struct ColorF
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;
	};

	// A point in the float coordinates the renderer draws in.
	struct PointF
	{
		float x = 0.0f;
		float y = 0.0f;
	};
}
