#pragma once

#include "TypeOfTheHorizontalCRS.h"
#include "VerticalCoordinateBase.h"
#include "VerticalDatumReference.h"

#include <hdf5.h>

class H5_RootGroup
{
public:
	H5_RootGroup();
	virtual ~H5_RootGroup();

public:
	std::string productSpecification;
	std::string* issueTime = nullptr;
	std::string issueDate;
	int horizontalCRS = 0;
	std::string* nameOfHorizontalCRS = nullptr;
	TypeOfTheHorizontalCRS* typeOfTheHorizontalCRS = nullptr;
	int* horizontalCS = nullptr;
	int* horizontalDatum = nullptr;
	std::string* nameOfHorizontalDatum = nullptr;
	int* primeMeridian = nullptr;
	int* spheroid = nullptr;
	int* projectionMethod = nullptr;
	double* projectionParameter1 = nullptr;
	double* projectionParameter2 = nullptr;
	double* projectionParameter3 = nullptr;
	double* projectionParameter4 = nullptr;
	double* projectionParameter5 = nullptr;
	double* falseNorthing = nullptr;
	double* falseEasting = nullptr;
	std::string* epoch = nullptr;
	double westBoundLongitude = 0;
	double eastBoundLongitude = 0;
	double southBoundLatitude = 0;
	double northBoundLatitude = 0;
	std::string* geographicIdentifier = nullptr;
	std::string metadata;
	int* verticalCS = nullptr;
	VerticalCoordinateBase* verticalCoordinateBase = nullptr;
	VerticalDatumReference* verticalDatumReference = nullptr;
	int* verticalDatum = nullptr;
	std::string* metaFeatures = nullptr;

public:
	std::string getProductSpecification() const;
	void setProductSpecification(const std::string& value);

	bool hasIssueTime() const;
	std::string getIssueTime() const;
	void setIssueTime(const std::string& value);

	std::string getIssueDate() const;
	void setIssueDate(const std::string& value);

	int getHorizontalCRS() const;
	void setHorizontalCRS(const int value);

	bool hasNameOfHorizontalCRS() const;
	std::string getNameOfHorizontalCRS() const;
	void setNameOfHorizontalCRS(const std::string value);

	bool hasTypeOfTheHorizontalCRS() const;
	TypeOfTheHorizontalCRS getTypeOfTheHorizontalCRS() const;
	void setTypeOfTheHorizontalCRS(const TypeOfTheHorizontalCRS value);

	bool hasHorizontalCS() const;
	int getHorizontalCS() const;
	void setHorizontalCS(const int value);

	bool hasHorizontalDatum() const;
	int getHorizontalDatum() const;
	void setHorizontalDatum(const int value);

	bool hasNameOfHorizontalDatum() const;
	std::string getNameOfHorizontalDatum() const;
	void setNameOfHorizontalDatum(const std::string value);

	bool hasPrimeMeridian() const;
	int getPrimeMeridian() const;
	void setPrimeMeridian(const int value);

	bool hasSpheroid() const;
	int getSpheroid() const;
	void setSpheroid(const int value);

	bool hasProjectionMethod() const;
	int getProjectionMethod() const;
	void setProjectionMethod(const int value);

	bool hasProjectionParameter1() const;
	double getProjectionParameter1() const;
	void setProjectionParameter1(const double value);

	bool hasProjectionParameter2() const;
	double getProjectionParameter2() const;
	void setProjectionParameter2(const double value);

	bool hasProjectionParameter3() const;
	double getProjectionParameter3() const;
	void setProjectionParameter3(const double value);

	bool hasProjectionParameter4() const;
	double getProjectionParameter4() const;
	void setProjectionParameter4(const double value);

	bool hasProjectionParameter5() const;
	double getProjectionParameter5() const;
	void setProjectionParameter5(const double value);

	bool hasFalseNorthing() const;
	double getFalseNorthing() const;
	void setFalseNorthing(const double value);

	bool hasFalseEasting() const;
	double getFalseEasting() const;
	void setFalseEasting(const double value);

	bool hasEpoch() const;
	std::string getEpoch() const;
	void setEpoch(const std::string value);

	double getWestBoundLongitude() const;
	void setWestBoundLongitude(const double value);

	double getEastBoundLongitude() const;
	void setEastBoundLongitude(const double value);

	double getSouthBoundLatitude() const;
	void setSouthBoundLatitude(const double value);

	double getNorthBoundLatitude() const;
	void setNorthBoundLatitude(const double value);

	bool hasGeographicIdentifier() const;
	std::string getGeographicIdentifier() const;
	void setGeographicIdentifier(const std::string value);

	std::string getMetadata() const;
	void setMetadata(const std::string value);

	bool hasVerticalCS() const;
	int getVerticalCS() const;
	void setVerticalCS(const int value);

	bool hasVerticalCoordinateBase() const;
	VerticalCoordinateBase getVerticalCoordinateBase() const;
	void setVerticalCoordinateBase(const VerticalCoordinateBase value);

	bool hasVerticalDatumReference() const;
	VerticalDatumReference getVerticalDatumReference() const;
	void setVerticalDatumReference(const VerticalDatumReference value);

	bool hasVerticalDatum() const;
	int getVerticalDatum() const;
	void setVerticalDatum(const int value);

	bool hasMetaFeatures() const;
	std::string getMetaFeatures() const;
	void setMetaFeatures(const std::string value);

public:
	virtual bool Read(hid_t rootID);
};

