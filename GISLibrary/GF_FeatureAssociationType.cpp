#include "stdafx.h"
#include "GF_FeatureAssociationType.h"

namespace GF
{
	FeatureAssociationType::FeatureAssociationType()
	{

	}

	FeatureAssociationType::~FeatureAssociationType()
	{

	}

	std::string FeatureAssociationType::GetFeatureID()
	{
		return featureID;
	}

	void FeatureAssociationType::SetFeatureID(std::string value)
	{
		featureID = value;
	}
}