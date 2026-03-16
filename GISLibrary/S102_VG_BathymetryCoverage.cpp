#include "stdafx.h"
#include "S102_VG_BathymetryCoverage.h"
#include <intsafe.h>

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
		// Layer 1: Input validation - reject invalid dimensions
		if (numLat <= 0 || numLon <= 0) {
			H5Tclose(type);
			H5Dclose(dsID);
			return false;
		}

		// Layer 2: Checked arithmetic - detect integer overflow
		size_t lat = static_cast<size_t>(numLat);
		size_t lon = static_cast<size_t>(numLon);
		size_t size;
		if (FAILED(SizeTMult(lat, lon, &size))) {
			H5Tclose(type);
			H5Dclose(dsID);
			return false;
		}

		// Layer 3: Maximum size limit - prevent resource exhaustion
		constexpr size_t MAX_GRID_POINTS = 100'000'000;
		if (size > MAX_GRID_POINTS) {
			H5Tclose(type);
			H5Dclose(dsID);
			return false;
		}

		// Layer 4: HDF5 cross-validation - detect metadata-dataset mismatch
		hid_t spaceID = H5Dget_space(dsID);
		hsize_t dims[2];
		H5Sget_simple_extent_dims(spaceID, dims, NULL);
		if (dims[0] != static_cast<hsize_t>(numLat) ||
			dims[1] != static_cast<hsize_t>(numLon)) {
			H5Sclose(spaceID);
			H5Tclose(type);
			H5Dclose(dsID);
			return false;
		}
		H5Sclose(spaceID);

		// Safe buffer allocation using HDF5 type introspection
		size_t elemSize = H5Tget_size(type);
		size_t bufSize = size * (elemSize / sizeof(float));
		float* buf = new float[bufSize];
		depth = new float[size];
		uncertainty = new float[size];
		memset(buf, 0, bufSize * sizeof(float));
		memset(depth, 0, size * sizeof(float));
		memset(uncertainty, 0, size * sizeof(float));
		H5Dread(dsID, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf);

		for (size_t i = 0; i < size; i++) {
			depth[i] = buf[i * 2];
			uncertainty[i] = buf[(i * 2) + 1];
		}

		delete[] buf;
	}

	H5Tclose(type);
	H5Dclose(dsID);
	return true;
}