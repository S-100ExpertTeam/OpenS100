#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

enum class SpatialPrimitiveType
{
	none = 0,
	point,
	pointSet,
	curve,
	surface,
	coverage,
	noGeometry
};

SpatialPrimitiveType StringToSpatialPrimitiveType(std::string& value);
SpatialPrimitiveType StringToSpatialPrimitiveType(std::wstring& value);

std::string SpatialPrimitiveTypeToString(SpatialPrimitiveType value);
std::wstring SpatialPrimitiveTypeToWString(SpatialPrimitiveType value);