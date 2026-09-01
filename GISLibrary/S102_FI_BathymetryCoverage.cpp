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
	if (vgID < 0) {
		return false;
	}

	auto bathymetryCoverage = GetBathymetryCoverage();

	// Propagate the result. Returning true unconditionally left callers to walk
	// depth[] and uncertainty[] after a rejected or failed read, which is how a
	// refused file turned into a null dereference further downstream.
	const bool read = bathymetryCoverage->Read(
		vgID,
		attribute29->getNumPointsLatitudinal(),
		attribute29->getNumPointsLongitudinal());

	H5Gclose(vgID);

	return read;
}

S102_VG_BathymetryCoverage* S102_FI_BathymetryCoverage::GetBathymetryCoverage()
{
	return (S102_VG_BathymetryCoverage*)valuesGroup.at(0);
}