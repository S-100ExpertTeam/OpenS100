#include "stdafx.h"
#include "Geometry.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <fstream>
#include <iostream>

Geometry::Geometry() 
{

}

Geometry::~Geometry()
{

}

MBR& Geometry::GetMBRRef()
{
	return m_mbr;
}

MBR Geometry::GetMBR()
{
	return m_mbr;
}

std::string Geometry::GetID()
{
	return id;
}

std::wstring Geometry::GetIDAsWString()
{
	return LibMFCUtil::StringToWString(GetID());
}

int Geometry::GetIDAsInt()
{
	return std::stoi(GetID());
}

void Geometry::SetID(std::string value)
{
	this->id = value;
}

void Geometry::SetID(std::wstring value)
{
	SetID(LibMFCUtil::WStringToString(value));
}

void Geometry::SetID(int value)
{
	SetID(std::to_string(value));
}

double Geometry::GetDistance(POINT _p1, POINT _p2)
{
	POINT p;

	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	return sqrt((double)((p.x * p.x) + (p.y * p.y)));
}

double Geometry::GetAngle(POINT _p1, POINT _p2)
{
	POINT p;
	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	double d = sqrt(((double)(p.x) * p.x) + (p.y * p.y));
	double temp = p.x / d;

	if (p.y < 0)
	{
		return (acos(-1.0) * 2) - acos(temp);
	}

	return acos(temp);
}

POINT Geometry::GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset)
{
	double angle = GetAngle(_p1, _p2);  // The angle of the line in the positive direction of the x-axis.

	POINT returnValue;
	returnValue.x = (LONG)(_p1.x + offset * cos(angle));
	returnValue.y = (LONG)(_p1.y + offset * sin(angle));

	return returnValue;
}

void Geometry::swap(int &val1, int &val2)
{
	int t = val1;
	val1 = val2;
	val2 = t;
}

bool Geometry::IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2)
{
	float x12 = (float)(_s1.x - _e1.x);
	float x34 = (float)(_s2.x - _e2.x);
	float y12 = (float)(_s1.y - _e1.y);
	float y34 = (float)(_s2.y - _e2.y);

	float c = x12 * y34 - y12 * x34;

	if (fabs(c) < 0.01)
	{
		// No intersection
		return false;
	}
	
	return true;
}

void Geometry::CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect *viewPort)
{
	// Store the values for fast access and easy

	// equations-to-code conversion
	float x1 = (float)_s1.x, x2 = (float)_e1.x, x3 = (float)_s2.x, x4 = (float)_e2.x;
	float y1 = (float)_s1.y, y2 = (float)_e1.y, y3 = (float)_s2.y, y4 = (float)_e2.y;
	float d = (float)(x1 - x2) * (float)(y3 - y4) - (float)(y1 - y2) * (float)(x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return;

	// Get the x and y
	float pre = (float)(x1*y2 - y1 * x2), post = (float)(x3*y4 - y3 * x4);
	float x = (float)(pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (float)(pre * (y3 - y4) - (y1 - y2) * post) / d;

	if (_s2.x == _s1.x ||
		_s2.x == _s1.x ||
		_s2.y == _s1.y ||
		_s2.y == _s1.y ||
		_e2.x == _e1.x ||
		_e2.x == _e1.x ||
		_e2.y == _e1.y ||
		_e2.y == _e1.y
		)
	{
		return;
	}
	else if (
		viewPort->left > _s1.x ||
		viewPort->right < _s1.x ||
		viewPort->bottom < _s1.y ||
		viewPort->top	> _s1.y)
	{
		_s1.x = (LONG)x;
		_s1.y = (LONG)y;
	}
	else
	{
		_e1.x = (LONG)x;
		_e1.y = (LONG)y;
	}
}

bool Geometry::WriteWkb(std::wstring path)
{
	unsigned char* wkb = nullptr;
	int size = 0;
	ExportToWkb(&wkb, &size);

	std::ofstream writeFile(path.data());
	if (writeFile.is_open())
	{
		writeFile.write((char*)wkb, size);
		writeFile.close();
	}

	return true;
}