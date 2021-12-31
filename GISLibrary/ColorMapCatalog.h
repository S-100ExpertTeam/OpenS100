#pragma once
#include "ColorMap.h"

#include <vector>

namespace nmColorMap 
{
	class ColorMapCatalog
	{
	public:
		ColorMapCatalog();
		virtual ~ColorMapCatalog();

	public:
		std::vector<nmColorMap::ColorMap> colorMap;

	public:
		int GetColorMapCount();
		nmColorMap::ColorMap GetColorMap(std::wstring _name);
		bool AddColorMap(nmColorMap::ColorMap _colorMap);
		bool DeleteColorMap(std::wstring _name);

	};
}