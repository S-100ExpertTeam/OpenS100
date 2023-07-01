#include "stdafx.h"
#include "S102_VG_BathymetryCoverage.h"

S102_VG_BathymetryCoverage::S102_VG_BathymetryCoverage()
{

}

S102_VG_BathymetryCoverage::~S102_VG_BathymetryCoverage()
{
	delete[] depth;
	delete[] uncertainty;
}

bool S102_VG_BathymetryCoverage::Read(hid_t groupID)
{
	return true;
}

bool S102_VG_BathymetryCoverage::Read(hid_t groupID, int numLat, int numLon)
{
	auto dsID = H5Dopen(groupID, "values", H5P_DEFAULT);
	if (dsID < 0) {
		return false;
	}

	auto type = H5Dget_type(dsID);
	if (H5Tget_class(type) == H5T_COMPOUND) {
		auto bufSize = numLat * numLon * 2;
		auto size = numLat * numLon;
		float* buf = new float[bufSize];
		depth = new float[size]; 
		uncertainty = new float[size];
		memset(buf, 0, bufSize * sizeof(float));
		memset(depth, 0, size * sizeof(float));
		memset(uncertainty, 0, size * sizeof(float));
		H5Dread(dsID, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf);
		
		for (int i = 0; i < size; i++) {
			depth[i] = buf[i * 2];
			uncertainty[i] = buf[(i * 2) + 1];
		}

		delete[] buf;
	}

	return true;
}