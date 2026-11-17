#pragma once
// Conversions between the value types of the drawing-instruction model and Direct2D.
// Render-side only: include it from sources that draw.
#include "DrawTypes.h"

#include <d2d1.h>

namespace GISLibrary
{
	inline D2D1_COLOR_F ToD2D(const ColorF& color)
	{
		return D2D1_COLOR_F{ color.r, color.g, color.b, color.a };
	}

	inline ColorF FromD2D(const D2D1_COLOR_F& color)
	{
		return ColorF{ color.r, color.g, color.b, color.a };
	}

	inline D2D1_POINT_2F ToD2D(const PointF& point)
	{
		return D2D1_POINT_2F{ point.x, point.y };
	}
}
