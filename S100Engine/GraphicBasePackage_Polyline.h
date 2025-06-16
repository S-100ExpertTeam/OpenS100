#pragma once
#include "GraphicBasePackage_Point.h"
#include "GraphicBasePackage_PathSegment.h"

#include <vector>

namespace GraphicBasePackage
{
	class Polyline : public PathSegment
	{
	public:
		Polyline() {};
		virtual ~Polyline() {};

	public:
		std::vector<Point> _point;
	};
}
