#pragma once

#include <hdf5.h>

class H5_FI_Attribute29
{
public:
	H5_FI_Attribute29();
	H5_FI_Attribute29(const H5_FI_Attribute29& other);
	virtual ~H5_FI_Attribute29();

public:
	double gridOriginLongitude = 0;
	double gridOriginLatitude = 0;
	double gridSpacingLongitudinal = 0;
	double gridSpacingLatitudinal = 0;
	int numPointsLongitudinal = 0;
	int numPointsLatitudinal = 0;
	std::string startSequence;

public:
	virtual H5_FI_Attribute29 operator=(const H5_FI_Attribute29& other);

public:
	double getGridOriginLongitude() const;
	void setGridOriginLongitude(const double value);

	double getGridOriginLatitude() const;
	void setGridOriginLatitude(const double value);

	double getGridSpacingLongitudinal() const;
	void setGridSpacingLongitudinal(const double value);

	double getGridSpacingLatitudinal() const;
	void setGridSpacingLatitudinal(const double value);

	int getNumPointsLongitudinal() const;
	void setNumPointsLongitudinal(const int value);

	int getNumPointsLatitudinal() const;
	void setNumPointsLatitudinal(const int value);

	std::string getStartSequence() const;
	void setStartSequence(const std::string& value);

public:
	bool Read(hid_t groupID);
};

