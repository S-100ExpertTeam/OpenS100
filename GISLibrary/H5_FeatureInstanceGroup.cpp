#include "stdafx.h"
#include "H5_FeatureInstanceGroup.h"
#include "HDF5Wrapper.h"

H5_FeatureInstanceGroup::H5_FeatureInstanceGroup()
{

}

H5_FeatureInstanceGroup::~H5_FeatureInstanceGroup()
{
	delete westBoundLongitude;
	delete eastBoundLongitude;
	delete southBoundLatitude;
	delete northBoundLatitude;
	delete attribute29;

	for (auto i = valuesGroup.begin(); i != valuesGroup.end(); i++) {
		delete (*i);
	}
}

bool H5_FeatureInstanceGroup::hasWestBoundLongitude() const
{
	return westBoundLongitude;
}

double H5_FeatureInstanceGroup::getWestBoundLongitude() const
{
	return *westBoundLongitude;
}

void H5_FeatureInstanceGroup::setWestBoundLongitude(const double value)
{
	if (!westBoundLongitude) {
		westBoundLongitude = new double();
	}

	*westBoundLongitude = value;
}

bool H5_FeatureInstanceGroup::hasEastBoundLongitude() const
{
	return eastBoundLongitude;
}

double H5_FeatureInstanceGroup::getEastBoundLongitude() const
{
	return *eastBoundLongitude;
}

void H5_FeatureInstanceGroup::setEastBoundLongitude(const double value)
{
	if (!eastBoundLongitude) {
		eastBoundLongitude = new double();
	}

	*eastBoundLongitude = value;
}

bool H5_FeatureInstanceGroup::hasSouthBoundLatitude() const
{
	return southBoundLatitude;
}

double H5_FeatureInstanceGroup::getSouthBoundLatitude() const
{
	return *southBoundLatitude;
}

void H5_FeatureInstanceGroup::setSouthBoundLatitude(const double value)
{
	if (!southBoundLatitude) {
		southBoundLatitude = new double();
	}

	*southBoundLatitude = value;
}

bool H5_FeatureInstanceGroup::hasNorthBoundLatitude() const
{
	return northBoundLatitude;
}

double H5_FeatureInstanceGroup::getNorthBoundLatitude() const
{
	return *northBoundLatitude;
}

void H5_FeatureInstanceGroup::setNorthBoundLatitude(const double value)
{
	if (!northBoundLatitude) {
		northBoundLatitude = new double();
	}

	*northBoundLatitude = value;
}

int H5_FeatureInstanceGroup::getNumGRP() const
{
	return numGRP;
}

void H5_FeatureInstanceGroup::setNumGRP(const int value)
{
	numGRP = value;
}

bool H5_FeatureInstanceGroup::Read(hid_t groupID, DataOrganizationIndex dataCodingFormat)
{
	double dWestBoundLongitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "westBoundLongitude", dWestBoundLongitude)) {
		setWestBoundLongitude(dWestBoundLongitude);
	}

	double dEastBoundLongitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "eastBoundLongitude", dEastBoundLongitude)) {
		setEastBoundLongitude(dEastBoundLongitude);
	}

	double dSouthBoundLatitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "southBoundLatitude", dSouthBoundLatitude)) {
		setSouthBoundLatitude(dSouthBoundLatitude);
	}

	double dNorthBountLatitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "northBoundLatitude", dNorthBountLatitude)) {
		setNorthBoundLatitude(dNorthBountLatitude);
	}

	int iNumGRP = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "numGRP", iNumGRP)) {
		setNumGRP(iNumGRP);
	}

	if (dataCodingFormat == DataOrganizationIndex::regularGrid) {
		attribute29 = new H5_FI_Attribute29();
		attribute29->Read(groupID);
	}

	return true;
}

double H5_FeatureInstanceGroup::getGridOriginLongitude() const
{
	if (attribute29) {
		return attribute29->getGridOriginLongitude();
	}

	return 0;
}

double H5_FeatureInstanceGroup::getGridOriginLatitude() const
{
	if (attribute29) {
		return attribute29->getGridOriginLatitude();
	}

	return 0;
}

double H5_FeatureInstanceGroup::getGridSpacingLongitudinal() const
{
	if (attribute29) {
		return attribute29->getGridSpacingLongitudinal();
	}

	return 0;
}

double H5_FeatureInstanceGroup::getGridSpacingLatitudinal() const
{
	if (attribute29) {
		return attribute29->getGridSpacingLatitudinal();
	}

	return 0;
}

int H5_FeatureInstanceGroup::getNumPointsLongitudinal() const
{
	if (attribute29) {
		return attribute29->getNumPointsLongitudinal();
	}

	return 0;
}

int H5_FeatureInstanceGroup::getNumPointsLatitudinal() const
{
	if (attribute29) {
		return attribute29->getNumPointsLatitudinal();
	}

	return 0;
}

std::string H5_FeatureInstanceGroup::getStartSequence() const
{
	if (attribute29) {
		return attribute29->getStartSequence();
	}

	return "";
}