#include "stdafx.h"
#include "GM_Curve.h"

namespace S100Geometry
{
	Curve::Curve()
	{
		segment.push_back(CurveSegment());
	}

	Curve::~Curve()
	{

	}

	GeometryType Curve::GetType()
	{
		return GeometryType::Curve;
	}
}