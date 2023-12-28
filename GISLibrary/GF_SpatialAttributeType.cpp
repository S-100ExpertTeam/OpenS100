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

	SpatialAttributeType* SpatialAttributeType::Clone() const
	{
		SpatialAttributeType* sat = new SpatialAttributeType();

		sat->code = code;

		sat->scaleMinimum = scaleMinimum;
		sat->scaleMaximum = scaleMaximum;
		sat->geometryID = geometryID;

		for (const auto& iter : maskReference)
		{
			MaskReference mr = iter;
			sat->maskReference.push_back(mr);
		}

		return sat;
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