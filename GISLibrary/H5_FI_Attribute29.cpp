#include "stdafx.h"
#include "H5_FI_Attribute29.h"
#include "HDF5Wrapper.h"

H5_FI_Attribute29::H5_FI_Attribute29()
{

}

H5_FI_Attribute29::~H5_FI_Attribute29()
{

}

double H5_FI_Attribute29::getGridOriginLongitude() const
{
	return gridOriginLongitude;
}

void H5_FI_Attribute29::setGridOriginLongitude(const double value)
{
	gridOriginLongitude = value;
}

double H5_FI_Attribute29::getGridOriginLatitude() const
{
	return gridOriginLatitude;
}

void H5_FI_Attribute29::setGridOriginLatitude(const double value)
{
	gridOriginLatitude = value;
}

double H5_FI_Attribute29::getGridSpacingLongitudinal() const
{
	return gridSpacingLongitudinal;
}

void H5_FI_Attribute29::setGridSpacingLongitudinal(const double value)
{
	gridSpacingLongitudinal = value;
}

double H5_FI_Attribute29::getGridSpacingLatitudinal() const
{
	return gridSpacingLatitudinal;
}

void H5_FI_Attribute29::setGridSpacingLatitudinal(const double value)
{
	gridSpacingLatitudinal = value;
}

int H5_FI_Attribute29::getNumPointsLongitudinal() const
{
	return numPointsLongitudinal;
}

void H5_FI_Attribute29::setNumPointsLongitudinal(const int value)
{
	numPointsLongitudinal = value;
}

int H5_FI_Attribute29::getNumPointsLatitudinal() const
{
	return numPointsLatitudinal;
}

void H5_FI_Attribute29::setNumPointsLatitudinal(const int value)
{
	numPointsLatitudinal = value;
}

std::string H5_FI_Attribute29::getStartSequence() const
{
	return startSequence;
}

void H5_FI_Attribute29::setStartSequence(const std::string& value)
{
	startSequence = value;
}

bool H5_FI_Attribute29::Read(hid_t groupID)
{
	double dGridOriginLongitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "gridOriginLongitude", dGridOriginLongitude)) {
		setGridOriginLongitude(dGridOriginLongitude);
	}

	double dGridOriginLatitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "gridOriginLatitude", dGridOriginLatitude)) {
		setGridOriginLatitude(dGridOriginLatitude);
	}

	double dGridSpacingLongitudinal = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "gridSpacingLongitudinal", dGridSpacingLongitudinal)) {
		setGridSpacingLongitudinal(dGridSpacingLongitudinal);
	}

	double dGridSpacingLatitudinal = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(groupID, "gridSpacingLatitudinal", dGridSpacingLatitudinal)) {
		setGridSpacingLatitudinal(dGridSpacingLatitudinal);
	}

	int iNumPointsLongitudinal = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "numPointsLongitudinal", iNumPointsLongitudinal)) {
		setNumPointsLongitudinal(iNumPointsLongitudinal);
	}

	int iNumPointsLatitudinal = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "numPointsLatitudinal", iNumPointsLatitudinal)) {
		setNumPointsLatitudinal(iNumPointsLatitudinal);
	}

	std::string strStartSequence;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(groupID, "startSequence", strStartSequence)) {
		setStartSequence(strStartSequence);
	}

	return true;
}