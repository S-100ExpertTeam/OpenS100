#include "stdafx.h"
#include "SpatialPrimitiveType.h"


#include <pugixml.hpp>

SpatialPrimitiveType StringToSpatialPrimitiveType(std::string& value)
{
	if (value.compare("point") == 0)
	{
		return SpatialPrimitiveType::point;
	}
	else if (value.compare("pointSet") == 0)
	{
		return SpatialPrimitiveType::pointSet;
	}
	else if (value.compare("curve") == 0)
	{
		return SpatialPrimitiveType::curve;
	}
	else if (value.compare("surface") == 0)
	{
		return SpatialPrimitiveType::surface;
	}
	else if (value.compare("coverage") == 0)
	{
		return SpatialPrimitiveType::coverage;
	}
	else if (value.compare("noGeometry") == 0)
	{
		return SpatialPrimitiveType::noGeometry;
	}

	return SpatialPrimitiveType::none;
}

std::string SpatialPrimitiveTypeToString(SpatialPrimitiveType value)
{
	if (value == SpatialPrimitiveType::point)
	{
		return "point";
	}
	else if (value == SpatialPrimitiveType::pointSet)
	{
		return "pointSet";
	}
	else if (value == SpatialPrimitiveType::curve)
	{
		return "curve";
	}
	else if (value == SpatialPrimitiveType::surface)
	{
		return "surface";
	}
	else if (value == SpatialPrimitiveType::coverage)
	{
		return "coverage";
	}
	else if (value == SpatialPrimitiveType::noGeometry)
	{
		return "noGeometry";
	}

	return "none";
}

std::string SpatialPrimitiveTypeToWString(SpatialPrimitiveType value)
{
	return SpatialPrimitiveTypeToString(value);
}