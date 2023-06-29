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

bool H5_FeatureInstanceGroup::Read(hid_t groupID)
{
	double dWestBoundLongitude = 0;
	//if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "type", iSequencingRuleType)) {
	//	setSequencingRuleType((CV_SequenceType)iSequencingRuleType);
	//}

	return true;
}