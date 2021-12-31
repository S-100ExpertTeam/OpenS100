#pragma once
#include "SymColor.h"

#include <vector>

namespace S100_SVG_D2D1_DLL
{
	class SymbolPath
	{
	public:
		SymbolPath();
		virtual ~SymbolPath();

	public:
		float width = 0;
		SymColor color;
		ID2D1PathGeometry* geom;
	};
}