#pragma once
#include <gdiplus.h>
#include <string>

namespace S100_SVG_D2D1_DLL
{
	class SymColor
	{
	public:
		SymColor();
		virtual ~SymColor();

	public:
		Gdiplus::Color c;
		bool fill = false;
		float opacity;
		std::wstring token;
	};
}