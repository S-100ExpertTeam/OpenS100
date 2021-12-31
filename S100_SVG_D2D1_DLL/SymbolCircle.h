#pragma once
#include "SymColor.h"

#include <vector>

namespace S100_SVG_D2D1_DLL
{
	class SymbolCircle
	{
	public:
		SymbolCircle();
		virtual ~SymbolCircle();

	public:
		SymColor color;
		ID2D1EllipseGeometry* geom = nullptr;
	};
}