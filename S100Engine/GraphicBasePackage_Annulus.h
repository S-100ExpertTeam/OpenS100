#pragma once
#include "GraphicBasePackage_Point.h"
#include "GraphicBasePackage_PathSegment.h"
#include "GraphicBasePackage_Sector.h"

namespace GraphicBasePackage
{
	class Annulus : public PathSegment
	{
	public:
		Annulus();
		virtual ~Annulus();

	public:
		Point _center;
		double* _innerRadius = nullptr;
		double _outerRadius = 0;
		Sector* _sector = nullptr;
	};
}