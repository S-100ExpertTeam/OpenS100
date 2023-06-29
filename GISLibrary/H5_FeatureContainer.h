#pragma once

#include "DataOrganizationIndex.h"
#include "CV_CommonPointRule.h"
#include "H5_FC_Attribute2.h"

class H5_FeatureContainer
{
public:
	H5_FeatureContainer();
	virtual ~H5_FeatureContainer();

public:
	DataOrganizationIndex dataCodingFormat = DataOrganizationIndex::none;
	int dimension = 0;
	CV_CommonPointRule commonPointRule = CV_CommonPointRule::none;
	double horizontalPositionUncertainty = 0;
	double verticalUncertainty = 0;
	double* timeUncertainty = nullptr;
	int numInstances = 0;

	H5_FC_Attribute2* attribute2 = nullptr;

public:
	DataOrganizationIndex getDataCodingFormat() const;
	void setDataCodingFormat(const DataOrganizationIndex value);

	int getDimension() const;
	void setDimension(const int value);

	CV_CommonPointRule getCommonPointRule() const;
	void setCommonPointRule(const CV_CommonPointRule value);

	int getHorizontalPositionUncertainty() const;
	void setHorizontalPositionUncertainty(const int value);

	int getVerticalUncertainty() const;
	void setVerticalUncertainty(const int value);

	bool hasTimeUncertainty() const;
	double getTimeUncertainty() const;
	void setTimeUncertainty(const double value);

	int getNumInstances() const;
	void setNumInstances(const int value);

public:
	bool Read(hid_t groupID);
};
