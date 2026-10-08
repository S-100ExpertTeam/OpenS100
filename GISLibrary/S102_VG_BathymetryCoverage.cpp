#include "stdafx.h"
#include "S102_VG_BathymetryCoverage.h"

#include <new>

namespace
{
	// Upper bound on the number of grid points accepted from a single coverage.
	//
	// S-102 Product Specification 11.2.2 recommends grids of roughly 600 x 600
	// (360,000 points), so this leaves a margin of more than two orders of
	// magnitude for high-resolution products while still rejecting the
	// billion-point dimensions that a hostile file would have to declare.
	//
	// Note for maintainers: at this bound a single coverage can still ask for
	// roughly 1.6 GB across the three buffers below. Lower it if the target
	// hardware has a tighter memory budget.
	constexpr size_t kMaxGridPoints = 100000000;

	// Multiplies two non-negative grid dimensions, reporting overflow instead of
	// wrapping. The inputs have already been checked to be positive.
	bool MultiplyGridDimensions(int numLat, int numLon, size_t& result)
	{
		const size_t lat = static_cast<size_t>(numLat);
		const size_t lon = static_cast<size_t>(numLon);

		if (lat != 0 && lon > (SIZE_MAX / lat))
		{
			result = 0;
			return false;
		}

		result = lat * lon;
		return true;
	}
}

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

// Reads the bathymetry grid described by groupID.
//
// numLat and numLon arrive from the numPointsLatitudinal / numPointsLongitudinal
// attributes of the file being opened, so they are attacker-influenced input on
// any deployment that opens a product it did not produce itself. They decide how
// much memory is allocated below, while H5Dread with H5S_ALL writes as many
// elements as the file's own dataspace declares. Nothing in the format couples
// those two quantities, so both are validated here before either is used.
bool S102_VG_BathymetryCoverage::Read(hid_t groupID, int numLat, int numLon)
{
	auto dsID = H5Dopen(groupID, "values", H5P_DEFAULT);
	if (dsID < 0) {
		return false;
	}

	// Reject dimensions that are absent, zero or negative before they reach any
	// size calculation.
	if (numLat <= 0 || numLon <= 0) {
		H5Dclose(dsID);
		return false;
	}

	auto type = H5Dget_type(dsID);
	if (type < 0) {
		H5Dclose(dsID);
		return false;
	}

	if (H5Tget_class(type) != H5T_COMPOUND) {
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	// The loop at the end of this function unpacks each record as two
	// consecutive floats, so a compound type of any other width would make the
	// indexing wrong. Check it rather than assume it.
	const size_t elemSize = H5Tget_size(type);
	if (elemSize != 2 * sizeof(float)) {
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	// Compute the declared point count with overflow reported rather than
	// wrapped. The original expression evaluated numLat * numLon * 2 in int,
	// where a product above INT_MAX wraps to a small value and yields an
	// allocation far smaller than the subsequent read.
	size_t pointCount = 0;
	if (!MultiplyGridDimensions(numLat, numLon, pointCount)) {
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	if (pointCount == 0 || pointCount > kMaxGridPoints) {
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	// Cross-check the declared dimensions against the dataspace that H5Dread
	// will actually use. H5S_ALL reads the whole dataset as described by the
	// file, independently of anything computed from the attributes above, so a
	// file whose metadata and dataspace disagree overflows the buffer even when
	// the arithmetic never overflows.
	auto spaceID = H5Dget_space(dsID);
	if (spaceID < 0) {
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	const int ndims = H5Sget_simple_extent_ndims(spaceID);
	const hssize_t storedPoints = H5Sget_simple_extent_npoints(spaceID);

	H5Sclose(spaceID);

	if (ndims != 2 || storedPoints < 0 ||
		static_cast<size_t>(storedPoints) != pointCount) {
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	// Only now is it safe to size the buffers from the metadata.
	const size_t floatCount = pointCount * 2;

	float* buf = new (std::nothrow) float[floatCount];
	float* newDepth = new (std::nothrow) float[pointCount];
	float* newUncertainty = new (std::nothrow) float[pointCount];

	if (buf == nullptr || newDepth == nullptr || newUncertainty == nullptr) {
		delete[] buf;
		delete[] newDepth;
		delete[] newUncertainty;
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	memset(buf, 0, floatCount * sizeof(float));
	memset(newDepth, 0, pointCount * sizeof(float));
	memset(newUncertainty, 0, pointCount * sizeof(float));

	if (H5Dread(dsID, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf) < 0) {
		delete[] buf;
		delete[] newDepth;
		delete[] newUncertainty;
		H5Tclose(type);
		H5Dclose(dsID);
		return false;
	}

	for (size_t i = 0; i < pointCount; i++) {
		newDepth[i] = buf[i * 2];
		newUncertainty[i] = buf[(i * 2) + 1];
	}

	delete[] buf;

	// Replace any previously read grid so that a second Read() does not leak.
	delete[] depth;
	delete[] uncertainty;
	depth = newDepth;
	uncertainty = newUncertainty;
	pointCountRead = pointCount;

	H5Tclose(type);
	H5Dclose(dsID);

	return true;
}
