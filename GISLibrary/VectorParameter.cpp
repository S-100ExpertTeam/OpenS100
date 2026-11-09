#include "stdafx.h"
#include "VectorParameter.h"

namespace Part9a
{

	void Vector::SetX(double value)
	{
		x = value;
	}

	double Vector::GetX() const
	{
		return x;
	}

	void Vector::SetY(double value)
	{
		y = value;
	}

	double Vector::GetY() const
	{
		return y;
	}

	void Vector::Set(double _x, double _y)
	{
		SetX(_x);
		SetY(_y);
	}

}