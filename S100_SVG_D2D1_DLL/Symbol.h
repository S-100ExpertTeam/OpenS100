#pragma once
#include "SymbolPath.h"
#include "SymbolCircle.h"

#include <vector>

namespace S100_SVG_D2D1_DLL
{
	class Symbol
	{
	public:
		Symbol();
		virtual ~Symbol();

	public:
		std::vector<SymbolPath> symPath;
		std::vector<SymbolCircle> symCircle;
		float width = 0;
		float height = 0;
	};
}