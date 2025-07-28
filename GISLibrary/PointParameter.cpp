#include "stdafx.h"
#include "PointParameter.h"

namespace DrawingInstructions
{
	Point::Point(double x, double y) : x(x), y(y)
	{
	}

	void Point::SetX(double value)
	{
		x = value;
	}

	double Point::GetX() const
	{
		return x;
	}

	void Point::SetY(double value)
	{
		y = value;
	}

	double Point::GetY() const
	{
		return y;
	}

	void Point::Set(double x, double y)
	{
		SetX(x);
		SetY(y);
	}
}
