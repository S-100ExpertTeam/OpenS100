#include "stdafx.h"
#include "H5_FeatureContainer.h"
#include "HDF5Wrapper.h"

H5_FeatureContainer::H5_FeatureContainer()
{

}

H5_FeatureContainer::~H5_FeatureContainer()
{
	for (auto i = featureInstanceGroup.begin(); i != featureInstanceGroup.end(); i++) {
		delete (*i);
	}

	delete timeUncertainty;
	delete attribute2;
}

bool H5_FeatureContainer::Read(hid_t groupID)
{
	int iDataCodingFormat = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "dataCodingFormat", iDataCodingFormat)) {
		setDataCodingFormat((DataOrganizationIndex)iDataCodingFormat);
	}

	int iDimension = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "dimension", iDimension)) {
		setDimension(iDimension);
	}

	int iCommonPointRule = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "commonPointRule", iCommonPointRule)) {
		setCommonPointRule((CV_CommonPointRule)iDimension);
	}

	double dHorizontalPositionUncertainty = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "horizontalPositionUncertainty", dHorizontalPositionUncertainty)) {
		setHorizontalPositionUncertainty(dHorizontalPositionUncertainty);
	}

	double dVerticalUncertainty = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "verticalUncertainty", dVerticalUncertainty)) {
		setVerticalUncertainty(dVerticalUncertainty);
	}

	double dTimeUncertainty = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "timeUncertainty", dTimeUncertainty)) {
		setTimeUncertainty(dTimeUncertainty);
	}

	int iNumInstances = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "numInstances", iNumInstances)) {
		setNumInstances(iNumInstances);
	}

	if (dataCodingFormat == DataOrganizationIndex::regularGrid) {
		attribute2 = new H5_FC_Attribute2();
		attribute2->Read(groupID);
	}

	return true;
}

DataOrganizationIndex H5_FeatureContainer::getDataCodingFormat() const
{
	return dataCodingFormat;
}

void H5_FeatureContainer::setDataCodingFormat(const DataOrganizationIndex value)
{
	dataCodingFormat = value;
}

int H5_FeatureContainer::getDimension() const
{
	return dimension;
}

void H5_FeatureContainer::setDimension(const int value)
{
	dimension = value;
}

CV_CommonPointRule H5_FeatureContainer::getCommonPointRule() const
{
	return commonPointRule;
}

void H5_FeatureContainer::setCommonPointRule(const CV_CommonPointRule value)
{
	commonPointRule = value;
}

int H5_FeatureContainer::getHorizontalPositionUncertainty() const
{
	return horizontalPositionUncertainty;
}

void H5_FeatureContainer::setHorizontalPositionUncertainty(const int value)
{
	horizontalPositionUncertainty = value;
}

int H5_FeatureContainer::getVerticalUncertainty() const
{
	return verticalUncertainty;
}

void H5_FeatureContainer::setVerticalUncertainty(const int value)
{
	verticalUncertainty = value;
}

bool H5_FeatureContainer::hasTimeUncertainty() const
{
	return timeUncertainty;
}

double H5_FeatureContainer::getTimeUncertainty() const
{
	if (timeUncertainty) {
		return *timeUncertainty;
	}

	return 0;
}

void H5_FeatureContainer::setTimeUncertainty(const double value)
{
	if (!timeUncertainty) {
		timeUncertainty = new double();
	}

	*timeUncertainty = value;
}

int H5_FeatureContainer::getNumInstances() const
{
	return numInstances;
}

void H5_FeatureContainer::setNumInstances(const int value)
{
	numInstances = value;
}