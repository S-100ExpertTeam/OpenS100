#include "stdafx.h"
#include "GF_SpatialAttributeType.h"

namespace GF
{
	SpatialAttributeType::SpatialAttributeType()
	{

	}

	SpatialAttributeType::~SpatialAttributeType()
	{

	}

	void SpatialAttributeType::SetGeometryID(std::string value)
	{
		geometryID = value;
	}

	std::string SpatialAttributeType::GetGeometryID()
	{
		return geometryID;
	}
}