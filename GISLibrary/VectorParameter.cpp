#include "stdafx.h"
#include "VectorParameter.h"

namespace DrawingCommand
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

	void Vector::Set(double x, double y)
	{
		SetX(x);
		SetY(y);
	}

}