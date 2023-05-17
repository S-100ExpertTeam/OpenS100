#include "stdafx.h"
#include "GF_FeatureType.h"

namespace GF
{
	FeatureType::FeatureType()
	{

	}

	FeatureType::~FeatureType()
	{

	}

	int FeatureType::GetAttributeCount()
	{
		return attributes.size();
	}

	void FeatureType::SetGeometryID(std::string value)
	{
		if (nullptr == spatial)
		{
			spatial = new SpatialAttributeType();
		}

		spatial->SetGeometryID(value);
	}
}