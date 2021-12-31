#include "stdafx.h"
#include "Color.h"

nmColorMap::Color::Color()
{
	color = RGB(255, 255, 255);
}


nmColorMap::Color::Color(int _r, int _g, int _b)
{
	color = RGB(_r, _g, _b);
}


nmColorMap::Color::Color(COLORREF _color)
{
	color = _color;
}


nmColorMap::Color::~Color()
{
}


int nmColorMap::Color::R()
{
	return GetRValue(color);
}


int nmColorMap::Color::G()
{
	return GetGValue(color);
}


int nmColorMap::Color::B()
{
	return GetBValue(color);
}


void nmColorMap::Color::Get(int& _r, int& _g, int& _b)
{
	_r = R();
	_g = G();
	_b = B();
}


void nmColorMap::Color::SetR(int _r)
{
	color = RGB(_r, G(), B());
}


void nmColorMap::Color::SetG(int _g)
{
	color = RGB(R(), _g, B());
}


void nmColorMap::Color::SetB(int _b)
{
	color = RGB(R(), G(), _b);
}


void nmColorMap::Color::Set(int _r, int _g, int _b)
{
	color = RGB(_r, _g, _b);
}


void nmColorMap::Color::Set(COLORREF _color)
{
	color = _color;
}
