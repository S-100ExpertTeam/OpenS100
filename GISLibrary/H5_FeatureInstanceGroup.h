#pragma once

#include "H5_FI_Attribute29.h"

#include <hdf5.h>

class H5_FeatureInstanceGroup
{
public:
	H5_FeatureInstanceGroup();
	virtual ~H5_FeatureInstanceGroup();

public:
	double* westBoundLongitude = nullptr;
	double* eastBoundLongitude = nullptr;
	double* southBoundLatitude = nullptr;
	double* northBoundLatitude = nullptr;
	int numGRP = 0;

	H5_FI_Attribute29* attribute29 = nullptr;

public:
	bool hasWestBoundLongitude() const;
	double getWestBoundLongitude() const;
	void setWestBoundLongitude(const double value);

	bool hasEastBoundLongitude() const;
	double getEastBoundLongitude() const;
	void setEastBoundLongitude(const double value);

	bool hasSouthBoundLatitude() const;
	double getSouthBoundLatitude() const;
	void setSouthBoundLatitude(const double value);

	bool hasNorthBoundLatitude() const;
	double getNorthBoundLatitude() const;
	void setNorthBoundLatitude(const double value);

	int getNumGRP() const;
	void setNumGRP(const int value);

public:
	bool Read(hid_t groupID);
};