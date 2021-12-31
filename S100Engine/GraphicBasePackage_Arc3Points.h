#pragma once
#include "GraphicBasePackage_Point.h"
#include "GraphicBasePackage_Arc.h"

namespace GraphicBasePackage
{
	class Arc3Points : public Arc
	{
	public:
		Arc3Points() {}
		virtual ~Arc3Points() {}

	public:
		Point _startPoint;
		Point _medianPoint;
		Point _endPoint;
	};
}