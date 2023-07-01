#include "stdafx.h"
#include "S102_FC_BathymetryCoverage.h"

S102_FC_BathymetryCoverage::S102_FC_BathymetryCoverage()
{
	featureInstanceGroup.push_back(new S102_FI_BathymetryCoverage());
}

S102_FC_BathymetryCoverage::~S102_FC_BathymetryCoverage()
{

}

bool S102_FC_BathymetryCoverage::Read(hid_t groupID)
{
	H5_FeatureContainer::Read(groupID);

	auto instanceID = H5Gopen(groupID, "BathymetryCoverage.01", H5P_DEFAULT);

	if (instanceID < 0)
	{
		instanceID = H5Gopen(groupID, "BathymetryCoverage_01", H5P_DEFAULT);
	}

	if (instanceID > 0)
	{
		auto bathymetryCoverage = GetBathymetryCoverage();
		if (!bathymetryCoverage->Read(instanceID, dataCodingFormat))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

S102_FI_BathymetryCoverage* S102_FC_BathymetryCoverage::GetBathymetryCoverage()
{
	return (S102_FI_BathymetryCoverage*)featureInstanceGroup.at(0);
}