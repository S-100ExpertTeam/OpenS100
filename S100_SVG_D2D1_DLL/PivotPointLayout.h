#pragma once
#include "DrawingPattern.h"
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class PivotPointLayout : public DrawingPattern
	{
	public:
		PivotPointLayout();
		virtual ~PivotPointLayout();

	public:
		std::wstring fill;
		std::wstring cx;
		std::wstring cy;
		std::wstring r;
	};
}