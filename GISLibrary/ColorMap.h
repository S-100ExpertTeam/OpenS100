#pragma once

#include <vector>
#include "Color.h"

namespace nmColorMap {

	class ColorMap
	{
	public:
		ColorMap();
		ColorMap(std::wstring _name);
		virtual ~ColorMap();

	public:
		std::wstring name;
		std::vector<nmColorMap::Color> color;

	public:
		int GetColorCount();
		void AddColor(nmColorMap::Color _color);
		void AddColor(COLORREF _color);
		void AddColor(int _r, int _g, int _b);
		void DeleteColor(int _index);
	};
}