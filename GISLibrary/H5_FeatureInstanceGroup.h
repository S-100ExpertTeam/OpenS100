#pragma once

#include "H5_FI_Attribute29.h"
#include "DataOrganizationIndex.h"
#include "H5_ValuesGroup.h"

#include <hdf5.h>
#include <vector>

class H5_FeatureInstanceGroup
{
public:
	H5_FeatureInstanceGroup();
	H5_FeatureInstanceGroup(const H5_FeatureInstanceGroup& other);
	virtual ~H5_FeatureInstanceGroup();

public:
	double* westBoundLongitude = nullptr;
	double* eastBoundLongitude = nullptr;
	double* southBoundLatitude = nullptr;
	double* northBoundLatitude = nullptr;
	int numGRP = 0;

	H5_FI_Attribute29* attribute29 = nullptr;

	std::vector<H5_ValuesGroup*> valuesGroup;

public:
	virtual H5_FeatureInstanceGroup operator=(const H5_FeatureInstanceGroup& other);

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

	// H5_FI_Attribute29
	double getGridOriginLongitude() const;
	double getGridOriginLatitude() const;
	double getGridSpacingLongitudinal() const;
	double getGridSpacingLatitudinal() const;
	int getNumPointsLongitudinal() const;
	int getNumPointsLatitudinal() const;
	std::string getStartSequence() const;

public:
	virtual bool Read(hid_t groupID, DataOrganizationIndex dataCodingFormat);
};