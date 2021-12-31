#include "stdafx.h"
#include "ColorMapCatalog.h"


nmColorMap::ColorMapCatalog::ColorMapCatalog()
{
}


nmColorMap::ColorMapCatalog::~ColorMapCatalog()
{
}


int nmColorMap::ColorMapCatalog::GetColorMapCount()
{
	return (int)colorMap.size();
}


nmColorMap::ColorMap nmColorMap::ColorMapCatalog::GetColorMap(std::wstring _name)
{
	for (unsigned i = 0; i < colorMap.size(); i++) {
		if (colorMap[i].name == _name) {
			return colorMap[i];
		}
	}

	ColorMap ret;
	return ret;
}


bool nmColorMap::ColorMapCatalog::AddColorMap(ColorMap _colorMap)
{
	for (unsigned i = 0; i < colorMap.size(); i++) {
		if (colorMap[i].name == _colorMap.name) {
			return false;
		}
	}
	
	colorMap.push_back(_colorMap);
	return true;
}


bool nmColorMap::ColorMapCatalog::DeleteColorMap(std::wstring _name)
{
	for (unsigned i = 0; i < colorMap.size(); i++) {
		if (colorMap[i].name == _name) {
			colorMap.erase(colorMap.begin() + i);
			return true;
		}
	}
	
	return false;
}