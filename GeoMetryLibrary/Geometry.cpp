#include "StdAfx.h"
#include "Geometry.h"

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

double Geometry::GetDistanceOfPolyline(POINT *_p, int _count)
{
	double returnValue = 0;

	for (int i = 1; i < _count; i++)
	{
		returnValue += GetDistance(_p[i - 1], _p[i]);
	}

	return returnValue;
}

inline void swap(int &val1, int &val2)
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
	else
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

void Geometry::GetViewLineInPolyline(POINT *_p, int _count, CRect *viewPort)
{
	POINT p1, p2;
	p1.x = viewPort->left;
	p1.y = viewPort->top;
	p2.x = viewPort->right;
	p2.y = viewPort->top;
	if (p1.y > _p[0].y)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.y > _p[_count - 1].y)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}

	p1.x = viewPort->right;
	p1.y = viewPort->top;
	p2.x = viewPort->right;
	p2.y = viewPort->bottom;
	if (p1.x < _p[0].x)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.x < _p[_count - 1].x)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}
	p1.x = viewPort->left;
	p1.y = viewPort->bottom;
	p2.x = viewPort->right;
	p2.y = viewPort->bottom;
	if (p1.y < _p[0].y)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.y < _p[_count - 1].y)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}
	p1.x = viewPort->left;
	p1.y = viewPort->top;
	p2.x = viewPort->left;
	p2.y = viewPort->bottom;

	if (p1.x > _p[0].x)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}

	else if (p1.x > _p[_count - 1].x)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}

	return;
}

POINT* Geometry::GetCenterPointOfPolyline(POINT *_p, int _count, CRect *viewPort)
{
	if (_count < 2)
	{
		return NULL;
	}
	else
	{
		GetViewLineInPolyline(_p, _count, viewPort);
	}

	double centerDistance = GetDistanceOfPolyline(_p, _count) / 2; // The distance from the starting point of the polyline to the middle point of the polyline.

	if (centerDistance == 0)
	{
		return NULL;
	}

	double accumulatedDistance = 0;								   // Accumulated distance.
	POINT *returnValue = new POINT;

	for (int i = 0; i < (_count - 1); i++)						   // Measure the cumulative distance from the 0th point to the next point.
	{
		accumulatedDistance += GetDistance(_p[i], _p[i + 1]);      // Move to the next point and measure the cumulative distance.

		if (accumulatedDistance >= centerDistance)				   // If the cumulative distance is greater than the intermediate distance, it has passed the midpoint.
		{
			*returnValue = GetOffsetPointOnLine(_p[i + 1], _p[i], accumulatedDistance - centerDistance); // It goes as far as the distance passed.
			break;
		}
	}

	return returnValue;
}

bool Geometry::WriteWkb(std::wstring path)
{
	char* wkb = nullptr;
	int size = 0;
	ExportToWkb(&wkb, &size);

	std::ofstream writeFile(path.data());
	if (writeFile.is_open())
	{
		writeFile.write(wkb, size);
		writeFile.close();
	}

	return true;
}