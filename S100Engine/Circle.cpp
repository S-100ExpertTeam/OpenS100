#include "stdafx.h"
#include "Circle.h"

namespace libS100Engine
{
	Circle::Circle()
	{
		initialize();
	}

	Circle::~Circle()
	{

	}

	void Circle::initialize()
	{
		type = FigureType::circle;
		cx = cy = radius = strokeWidth = rotation = 0;
	}
}