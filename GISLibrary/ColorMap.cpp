#include "stdafx.h"
#include "ColorMap.h"


nmColorMap::ColorMap::ColorMap()
{
}


nmColorMap::ColorMap::ColorMap(std::wstring _name)
{
	name = _name;
}


nmColorMap::ColorMap::~ColorMap()
{
}


int nmColorMap::ColorMap::GetColorCount()
{
	return (int)color.size();
}


void nmColorMap::ColorMap::AddColor(Color _color)
{
	color.push_back(_color);
}


void nmColorMap::ColorMap::AddColor(COLORREF _color)
{
	Color tmpColor(_color);
	color.push_back(tmpColor);
}


void nmColorMap::ColorMap::AddColor(int _r, int _g, int _b)
{
	Color tmpColor(_r, _g, _b);
	color.push_back(tmpColor);
}


void nmColorMap::ColorMap::DeleteColor(int _index)
{
	color.erase(color.begin() + _index);
}