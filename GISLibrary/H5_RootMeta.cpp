#include "stdafx.h"
#include "H5_RootMeta.h"
#include "HDF5Wrapper.h"

H5_RootMeta::H5_RootMeta()
{

}

H5_RootMeta::~H5_RootMeta()
{
	delete issueTime;
	delete nameOfHorizontalCRS;
	delete typeOfTheHorizontalCRS;
	delete horizontalCS;
	delete horizontalDatum;
	delete nameOfHorizontalDatum;
	delete primeMeridian;
	delete spheroid;
	delete projectionMethod;
	delete projectionParameter1;
	delete projectionParameter2;
	delete projectionParameter3;
	delete projectionParameter4;
	delete projectionParameter5;
	delete falseNorthing;
	delete falseEasting;
	delete epoch;
	delete geographicIdentifier;
	delete verticalCS;
	delete verticalCoordinateBase;
	delete verticalDatumReference;
	delete verticalDatum;
	delete metaFeatures;
}

std::string H5_RootMeta::getProductSpecification() const
{
	return productSpecification;
}

void H5_RootMeta::setProductSpecification(const std::string& value)
{
	productSpecification = value;
}

bool H5_RootMeta::hasIssueTime() const
{
	return issueTime;
}

std::string H5_RootMeta::getIssueTime() const
{
	return *issueTime;
}

void H5_RootMeta::setIssueTime(const std::string& value)
{
	if (!issueTime)
	{
		issueTime = new std::string();
	}

	*issueTime = value;
}

std::string H5_RootMeta::getIssueDate() const
{
	return issueDate;
}

void H5_RootMeta::setIssueDate(const std::string& value)
{
	issueDate = value;
}

int H5_RootMeta::getHorizontalCRS() const
{
	return horizontalCRS;
}

void H5_RootMeta::setHorizontalCRS(const int value)
{
	horizontalCRS = value;
}

bool H5_RootMeta::hasNameOfHorizontalCRS() const
{
	return nameOfHorizontalCRS;
}

std::string H5_RootMeta::getNameOfHorizontalCRS() const
{
	return *nameOfHorizontalCRS;
}

void H5_RootMeta::setNameOfHorizontalCRS(const std::string value)
{
	if (!nameOfHorizontalCRS)
	{
		nameOfHorizontalCRS = new std::string();
	}

	*nameOfHorizontalCRS = value;
}

bool H5_RootMeta::hasTypeOfTheHorizontalCRS() const
{
	return typeOfTheHorizontalCRS;
}

TypeOfTheHorizontalCRS H5_RootMeta::getTypeOfTheHorizontalCRS() const
{
	return *typeOfTheHorizontalCRS;
}

void H5_RootMeta::setTypeOfTheHorizontalCRS(const TypeOfTheHorizontalCRS value)
{
	if (!typeOfTheHorizontalCRS)
	{
		typeOfTheHorizontalCRS = new TypeOfTheHorizontalCRS();
	}

	*typeOfTheHorizontalCRS = value;
}

bool H5_RootMeta::hasHorizontalCS() const
{
	return horizontalCS;
}

int H5_RootMeta::getHorizontalCS() const
{
	return *horizontalCS;
}

void H5_RootMeta::setHorizontalCS(const int value)
{
	if (!horizontalCS)
	{
		horizontalCS = new int();
	}

	*horizontalCS = value;
}

bool H5_RootMeta::hasHorizontalDatum() const
{
	return horizontalDatum;
}

int H5_RootMeta::getHorizontalDatum() const
{
	return *horizontalDatum;
}

void H5_RootMeta::setHorizontalDatum(const int value)
{
	if (!horizontalDatum)
	{
		horizontalDatum = new int();
	}

	*horizontalDatum = value;
}

bool H5_RootMeta::hasNameOfHorizontalDatum() const
{
	return nameOfHorizontalDatum;
}

std::string H5_RootMeta::getNameOfHorizontalDatum() const
{
	return *nameOfHorizontalDatum;
}

void H5_RootMeta::setNameOfHorizontalDatum(const std::string value)
{
	if (!nameOfHorizontalDatum)
	{
		nameOfHorizontalDatum = new std::string();
	}

	*nameOfHorizontalDatum = value;
}

bool H5_RootMeta::hasPrimeMeridian() const
{
	return primeMeridian;
}

int H5_RootMeta::getPrimeMeridian() const
{
	return *primeMeridian;
}

void H5_RootMeta::setPrimeMeridian(const int value)
{
	if (!primeMeridian)
	{
		primeMeridian = new int();
	}

	*primeMeridian = value;
}

bool H5_RootMeta::hasSpheroid() const
{
	return spheroid;
}

int H5_RootMeta::getSpheroid() const
{
	return *spheroid;
}

void H5_RootMeta::setSpheroid(const int value)
{
	if (!spheroid)
	{
		spheroid = new int();
	}

	*spheroid = value;
}

bool H5_RootMeta::hasProjectionMethod() const
{
	return projectionMethod;
}

int H5_RootMeta::getProjectionMethod() const
{
	return *projectionMethod;
}

void H5_RootMeta::setProjectionMethod(const int value)
{
	if (!projectionMethod)
	{
		projectionMethod = new int();
	}

	*projectionMethod = value;
}

bool H5_RootMeta::hasProjectionParameter1() const
{
	return projectionParameter1;
}

double H5_RootMeta::getProjectionParameter1() const
{
	return *projectionParameter1;
}

void H5_RootMeta::setProjectionParameter1(const double value)
{
	if (!projectionParameter1)
	{
		projectionParameter1 = new double();
	}

	*projectionParameter1 = value;
}

bool H5_RootMeta::hasProjectionParameter2() const
{
	return projectionParameter2;
}

double H5_RootMeta::getProjectionParameter2() const
{
	return *projectionParameter2;
}

void H5_RootMeta::setProjectionParameter2(const double value)
{
	if (!projectionParameter2)
	{
		projectionParameter2 = new double();
	}

	*projectionParameter2 = value;
}

bool H5_RootMeta::hasProjectionParameter3() const
{
	return projectionParameter3;
}

double H5_RootMeta::getProjectionParameter3() const
{
	return *projectionParameter3;
}

void H5_RootMeta::setProjectionParameter3(const double value)
{
	if (!projectionParameter3)
	{
		projectionParameter3 = new double();
	}

	*projectionParameter3 = value;
}

bool H5_RootMeta::hasProjectionParameter4() const
{
	return projectionParameter4;
}

double H5_RootMeta::getProjectionParameter4() const
{
	return *projectionParameter4;
}

void H5_RootMeta::setProjectionParameter4(const double value)
{
	if (!projectionParameter4)
	{
		projectionParameter4 = new double();
	}

	*projectionParameter4 = value;
}

bool H5_RootMeta::hasProjectionParameter5() const
{
	return projectionParameter5;
}

double H5_RootMeta::getProjectionParameter5() const
{
	return *projectionParameter5;
}

void H5_RootMeta::setProjectionParameter5(const double value)
{
	if (!projectionParameter5)
	{
		projectionParameter5 = new double();
	}

	*projectionParameter5 = value;
}

bool H5_RootMeta::hasFalseNorthing() const
{
	return falseNorthing;
}

double H5_RootMeta::getFalseNorthing() const
{
	return *falseNorthing;
}

void H5_RootMeta::setFalseNorthing(const double value)
{
	if (!falseNorthing)
	{
		falseNorthing = new double();
	}

	*falseNorthing = value;
}

bool H5_RootMeta::hasFalseEasting() const
{
	return falseEasting;
}

double H5_RootMeta::getFalseEasting() const
{
	return *falseEasting;
}

void H5_RootMeta::setFalseEasting(const double value)
{
	if (!falseEasting)
	{
		falseEasting = new double();
	}

	*falseEasting = value;
}

bool H5_RootMeta::hasEpoch() const
{
	return epoch;
}

std::string H5_RootMeta::getEpoch() const
{
	return *epoch;
}

void H5_RootMeta::setEpoch(const std::string value)
{
	if (!epoch)
	{
		epoch = new std::string();
	}

	*epoch = value;
}

double H5_RootMeta::getWestBoundLongitude() const
{
	return westBoundLongitude;
}

void H5_RootMeta::setWestBoundLongitude(const double value) 
{
	westBoundLongitude = value;
}

double H5_RootMeta::getEastBoundLongitude() const
{
	return eastBoundLongitude;
}

void H5_RootMeta::setEastBoundLongitude(const double value)
{
	eastBoundLongitude = value;
}

double H5_RootMeta::getSouthBoundLatitude() const
{
	return southBoundLatitude;
}

void H5_RootMeta::setSouthBoundLatitude(const double value)
{
	southBoundLatitude = value;
}

double H5_RootMeta::getNorthBoundLatitude() const
{
	return northBoundLatitude;
}

void H5_RootMeta::setNorthBoundLatitude(const double value)
{
	northBoundLatitude = value;
}

bool H5_RootMeta::hasGeographicIdentifier() const
{
	return geographicIdentifier;
}

std::string H5_RootMeta::getGeographicIdentifier() const
{
	return *geographicIdentifier;
}

void H5_RootMeta::setGeographicIdentifier(const std::string value)
{
	if (!geographicIdentifier)
	{
		geographicIdentifier = new std::string();
	}

	*geographicIdentifier = value;
}

std::string H5_RootMeta::getMetadata() const
{
	return metadata;
}

void H5_RootMeta::setMetadata(const std::string value)
{
	metadata = value;
}

bool H5_RootMeta::hasVerticalCS() const
{
	return verticalCS;
}

int H5_RootMeta::getVerticalCS() const
{
	return *verticalCS;
}

void H5_RootMeta::setVerticalCS(const int value)
{
	if (!verticalCS)
	{
		verticalCS = new int();
	}

	*verticalCS = value;
}

bool H5_RootMeta::hasVerticalCoordinateBase() const
{
	return verticalCoordinateBase;
}

VerticalCoordinateBase H5_RootMeta::getVerticalCoordinateBase() const
{
	return *verticalCoordinateBase;
}

void H5_RootMeta::setVerticalCoordinateBase(const VerticalCoordinateBase value)
{
	if (!verticalCoordinateBase)
	{
		verticalCoordinateBase = new VerticalCoordinateBase();
	}

	*verticalCoordinateBase = value;
}

bool H5_RootMeta::hasVerticalDatumReference() const
{
	return verticalDatumReference;
}

VerticalDatumReference H5_RootMeta::getVerticalDatumReference() const
{
	return *verticalDatumReference;
}

void H5_RootMeta::setVerticalDatumReference(const VerticalDatumReference value)
{
	if (!verticalDatumReference)
	{
		verticalDatumReference = new VerticalDatumReference();
	}

	*verticalDatumReference = value;
}

bool H5_RootMeta::hasVerticalDatum() const
{
	return verticalDatum;
}

int H5_RootMeta::getVerticalDatum() const
{
	return *verticalDatum;
}

void H5_RootMeta::setVerticalDatum(const int value)
{
	if (!verticalDatum)
	{
		verticalDatum = new int();
	}

	*verticalDatum = value;
}

bool H5_RootMeta::hasMetaFeatures() const
{
	return metaFeatures;
}

std::string H5_RootMeta::getMetaFeatures() const
{
	return *metaFeatures;
}

void H5_RootMeta::setMetaFeatures(const std::string value)
{
	if (!metaFeatures)
	{
		metaFeatures = new std::string();
	}

	*metaFeatures = value;
}

bool H5_RootMeta::Read(hid_t rootID)
{
	std::string strPS;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "productSpecification", strPS)) {
		setProductSpecification(strPS);
	}
	
	std::string strIssueTime;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "issueTime", strIssueTime) ||
		HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "timeOfIssue", strIssueTime)) {
		setIssueTime(strIssueTime);
	}

	std::string strIssueDate;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "issueDate", strIssueDate)) {
		setIssueDate(strIssueDate);
	}

	int iHorizontalCRS = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "horizontalCRS", iHorizontalCRS)) {
		setHorizontalCRS(iHorizontalCRS);
	}

	std::string strNameOfHorizontalCRS;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "nameOfHorizontalCRS", strNameOfHorizontalCRS)) {
		setNameOfHorizontalCRS(strNameOfHorizontalCRS);
	}

	int iTypeOfHorizontalCRS = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "typeOfHorizontalCRS", iTypeOfHorizontalCRS)) {
		setTypeOfTheHorizontalCRS((TypeOfTheHorizontalCRS)iTypeOfHorizontalCRS);
	}

	int iHorizontalCS = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "horizontalCS", iHorizontalCS)) {
		setHorizontalCS(iHorizontalCS);
	}

	int iHorizontalDatum = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "horizontalDatum", iHorizontalDatum) || 
		HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "horizontalDatumValue", iHorizontalDatum)) {
		setHorizontalDatum(iHorizontalDatum);
	}

	std::string strNameOfHorizontalDatum;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "nameOfHorizontalDatum", strNameOfHorizontalDatum)) {
		setNameOfHorizontalDatum(strNameOfHorizontalDatum);
	}

	int iPrimeMeridian = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "primeMeridian", iPrimeMeridian)) {
		setPrimeMeridian(iPrimeMeridian);
	}

	int iSpheroid = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "spheroid", iSpheroid)) {
		setSpheroid(iSpheroid);
	}

	int iProjectionMethod = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "projectionMethod", iProjectionMethod)) {
		setProjectionMethod(iProjectionMethod);
	}

	double fProjectionParameter1 = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "projectionParameter1", fProjectionParameter1)) {
		setProjectionParameter1(fProjectionParameter1);
	}

	double fProjectionParameter2 = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "projectionParameter2", fProjectionParameter2)) {
		setProjectionParameter2(fProjectionParameter2);
	}

	double fProjectionParameter3 = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "projectionParameter3", fProjectionParameter3)) {
		setProjectionParameter3(fProjectionParameter3);
	}

	double fProjectionParameter4 = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "projectionParameter4", fProjectionParameter4)) {
		setProjectionParameter4(fProjectionParameter4);
	}

	double fProjectionParameter5 = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "projectionParameter5", fProjectionParameter5)) {
		setProjectionParameter5(fProjectionParameter5);
	}

	double fFalseNorthing = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "falseNorthing", fFalseNorthing)) {
		setFalseNorthing(fFalseNorthing);
	}

	double fFalseEasting = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "falseEasting", fFalseEasting)) {
		setFalseEasting(fFalseEasting);
	}

	std::string strEpoch;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "epoch", strEpoch)) {
		setEpoch(strEpoch);
	}

	double fWestBoundLongitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "westBoundLongitude", fWestBoundLongitude)) {
		setWestBoundLongitude(fWestBoundLongitude);
	}

	double fEastBoundLongitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "eastBoundLongitude", fEastBoundLongitude)) {
		setEastBoundLongitude(fEastBoundLongitude);
	}

	double fSouthBoundLatitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "southBoundLatitude", fSouthBoundLatitude)) {
		setSouthBoundLatitude(fSouthBoundLatitude);
	}

	double fNorthBoundLatitude = 0;
	if (HDF5Wrapper::ReadMetadataDoubleAttribute(rootID, "northBoundLatitude", fNorthBoundLatitude)) {
		setNorthBoundLatitude(fNorthBoundLatitude);
	}

	std::string strGeographicIdentifier;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "geographicIdentifier", strGeographicIdentifier)) {
		setGeographicIdentifier(strGeographicIdentifier);
	}

	std::string strMetadata;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "metadata", strMetadata)) {
		setMetadata(strMetadata);
	}

	int iVerticalCS = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "verticalCS", iVerticalCS)) {
		setVerticalCS(iVerticalCS);
	}

	int iVerticalCoordinateBase = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "verticalCoordinateBase", iVerticalCoordinateBase)) {
		setVerticalCoordinateBase((VerticalCoordinateBase)iVerticalCoordinateBase);
	}

	int iVerticalDatumReference = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "verticalDatumReference", iVerticalDatumReference)) {
		setVerticalDatumReference((VerticalDatumReference)iVerticalDatumReference);
	}

	int iVerticalDatum = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "verticalDatum", iVerticalDatum)) {
		setVerticalDatum(iVerticalDatum);
	}

	std::string strMetaFeatures;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(rootID, "metaFeatures", strMetaFeatures)) {
		setMetaFeatures(strMetaFeatures);
	}

	return true;
}