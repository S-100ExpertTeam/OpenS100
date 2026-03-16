#include "stdafx.h"
#include "S102_FI_BathymetryCoverage.h"

S102_FI_BathymetryCoverage::S102_FI_BathymetryCoverage()
{
	valuesGroup.push_back(new S102_VG_BathymetryCoverage());
}

S102_FI_BathymetryCoverage::~S102_FI_BathymetryCoverage()
{

}

bool S102_FI_BathymetryCoverage::Read(hid_t groupID, DataOrganizationIndex dataCodingFormat)
{
	H5_FeatureInstanceGroup::Read(groupID, dataCodingFormat);

	auto vgID = H5Gopen(groupID, "Group_001", H5P_DEFAULT);
	auto bathymetryCoverage = GetBathymetryCoverage();
	bool result = bathymetryCoverage->Read(
		vgID,
		attribute29->getNumPointsLatitudinal(),
		attribute29->getNumPointsLongitudinal());
	H5Gclose(vgID);
	return result;
}

S102_VG_BathymetryCoverage* S102_FI_BathymetryCoverage::GetBathymetryCoverage()
{
	return (S102_VG_BathymetryCoverage*)valuesGroup.at(0);
}