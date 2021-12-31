#pragma once
#include "S100_LinePen.h"
#include "S100_LineDash.h"
#include "S100_LineSymbol.h"

#include <vector>

namespace S100_SVG_D2D1_DLL 
{
	class S100_Line //No duplicates.
	{
	public:
		S100_Line();
		virtual ~S100_Line();

	public:
		float length;
		S100_LinePen pen;
		std::vector<S100_LineDash> dash;
		std::vector<S100_LineSymbol> sym;
	};
}