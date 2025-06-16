#pragma once
#include "GraphicBasePackage_Point.h"
#include "GraphicBasePackage_Sector.h"
#include "GraphicBasePackage_Arc.h"

namespace GraphicBasePackage
{
	class ArcByRadius : public Arc
	{
	public:
		ArcByRadius();
		virtual ~ArcByRadius();

	public:
		Point _center;
		Sector* _sector = nullptr;
		double _radius = 0;
	};
}