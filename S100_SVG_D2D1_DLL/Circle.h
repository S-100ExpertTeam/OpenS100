#pragma once
#include "DrawingPattern.h"

namespace S100_SVG_D2D1_DLL
{
	class Circle : public DrawingPattern
	{
	public:
		Circle();
		virtual ~Circle();

	public:
		std::wstring cx;
		std::wstring cy;
		std::wstring r;
	};
}