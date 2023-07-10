#include "stdafx.h"
#include "H5_RootGroup.h"
#include "HDF5Wrapper.h"

H5_RootGroup::H5_RootGroup()
{

}

H5_RootGroup::~H5_RootGroup()
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

std::string H5_RootGroup::getProductSpecification() const
{
	return productSpecification;
}

void H5_RootGroup::setProductSpecification(const std::string& value)
{
	productSpecification = value;
}

bool H5_RootGroup::hasIssueTime() const
{
	return issueTime;
}

std::string H5_RootGroup::getIssueTime() const
{
	if (issueTime) {
		return *issueTime;
	}

	return "";
}

void H5_RootGroup::setIssueTime(const std::string& value)
{
	if (!issueTime)
	{
		issueTime = new std::string();
	}

	*issueTime = value;
}

std::string H5_RootGroup::getIssueDate() const
{
	return issueDate;
}

void H5_RootGroup::setIssueDate(const std::string& value)
{
	issueDate = value;
}

int H5_RootGroup::getHorizontalCRS() const
{
	return horizontalCRS;
}

void H5_RootGroup::setHorizontalCRS(const int value)
{
	horizontalCRS = value;
}

bool H5_RootGroup::hasNameOfHorizontalCRS() const
{
	return nameOfHorizontalCRS;
}

std::string H5_RootGroup::getNameOfHorizontalCRS() const
{
	return *nameOfHorizontalCRS;
}

void H5_RootGroup::setNameOfHorizontalCRS(const std::string value)
{
	if (!nameOfHorizontalCRS)
	{
		nameOfHorizontalCRS = new std::string();
	}

	*nameOfHorizontalCRS = value;
}

bool H5_RootGroup::hasTypeOfTheHorizontalCRS() const
{
	return typeOfTheHorizontalCRS;
}

TypeOfTheHorizontalCRS H5_RootGroup::getTypeOfTheHorizontalCRS() const
{
	if (typeOfTheHorizontalCRS) {
		return *typeOfTheHorizontalCRS;
	}

	return TypeOfTheHorizontalCRS::none;
}

void H5_RootGroup::setTypeOfTheHorizontalCRS(const TypeOfTheHorizontalCRS value)
{
	if (!typeOfTheHorizontalCRS)
	{
		typeOfTheHorizontalCRS = new TypeOfTheHorizontalCRS();
	}

	*typeOfTheHorizontalCRS = value;
}

bool H5_RootGroup::hasHorizontalCS() const
{
	return horizontalCS;
}

int H5_RootGroup::getHorizontalCS() const
{
	if (horizontalCS) {
		return *horizontalCS;
	}

	return 0;
}

void H5_RootGroup::setHorizontalCS(const int value)
{
	if (!horizontalCS)
	{
		horizontalCS = new int();
	}

	*horizontalCS = value;
}

bool H5_RootGroup::hasHorizontalDatum() const
{
	return horizontalDatum;
}

int H5_RootGroup::getHorizontalDatum() const
{
	if (horizontalDatum) {
		return *horizontalDatum;
	}

	return 0;
}

void H5_RootGroup::setHorizontalDatum(const int value)
{
	if (!horizontalDatum)
	{
		horizontalDatum = new int();
	}

	*horizontalDatum = value;
}

bool H5_RootGroup::hasNameOfHorizontalDatum() const
{
	return nameOfHorizontalDatum;
}

std::string H5_RootGroup::getNameOfHorizontalDatum() const
{
	if (nameOfHorizontalDatum) {
		return *nameOfHorizontalDatum;
	}

	return "";
}

void H5_RootGroup::setNameOfHorizontalDatum(const std::string value)
{
	if (!nameOfHorizontalDatum)
	{
		nameOfHorizontalDatum = new std::string();
	}

	*nameOfHorizontalDatum = value;
}

bool H5_RootGroup::hasPrimeMeridian() const
{
	return primeMeridian;
}

int H5_RootGroup::getPrimeMeridian() const
{
	if (primeMeridian) {
		return *primeMeridian;
	}

	return 0;
}

void H5_RootGroup::setPrimeMeridian(const int value)
{
	if (!primeMeridian)
	{
		primeMeridian = new int();
	}

	*primeMeridian = value;
}

bool H5_RootGroup::hasSpheroid() const
{
	return spheroid;
}

int H5_RootGroup::getSpheroid() const
{
	if (spheroid) {
		return *spheroid;
	}

	return 0;
}

void H5_RootGroup::setSpheroid(const int value)
{
	if (!spheroid)
	{
		spheroid = new int();
	}

	*spheroid = value;
}

bool H5_RootGroup::hasProjectionMethod() const
{
	return projectionMethod;
}

int H5_RootGroup::getProjectionMethod() const
{
	if (projectionMethod) {
		return *projectionMethod;
	}

	return 0;
}

void H5_RootGroup::setProjectionMethod(const int value)
{
	if (!projectionMethod)
	{
		projectionMethod = new int();
	}

	*projectionMethod = value;
}

bool H5_RootGroup::hasProjectionParameter1() const
{
	return projectionParameter1;
}

double H5_RootGroup::getProjectionParameter1() const
{
	if (projectionParameter1) {
		return *projectionParameter1;
	}

	return 0;
}

void H5_RootGroup::setProjectionParameter1(const double value)
{
	if (!projectionParameter1)
	{
		projectionParameter1 = new double();
	}

	*projectionParameter1 = value;
}

bool H5_RootGroup::hasProjectionParameter2() const
{
	return projectionParameter2;
}

double H5_RootGroup::getProjectionParameter2() const
{
	if (projectionParameter2) {
		return *projectionParameter2;
	}

	return 0;
}

void H5_RootGroup::setProjectionParameter2(const double value)
{
	if (!projectionParameter2)
	{
		projectionParameter2 = new double();
	}

	*projectionParameter2 = value;
}

bool H5_RootGroup::hasProjectionParameter3() const
{
	return projectionParameter3;
}

double H5_RootGroup::getProjectionParameter3() const
{
	if (projectionParameter3) {
		return *projectionParameter3;
	}

	return 0;
}

void H5_RootGroup::setProjectionParameter3(const double value)
{
	if (!projectionParameter3)
	{
		projectionParameter3 = new double();
	}

	*projectionParameter3 = value;
}

bool H5_RootGroup::hasProjectionParameter4() const
{
	return projectionParameter4;
}

double H5_RootGroup::getProjectionParameter4() const
{
	if (projectionParameter4) {
		return *projectionParameter4;
	}

	return 0;
}

void H5_RootGroup::setProjectionParameter4(const double value)
{
	if (!projectionParameter4)
	{
		projectionParameter4 = new double();
	}

	*projectionParameter4 = value;
}

bool H5_RootGroup::hasProjectionParameter5() const
{
	return projectionParameter5;
}

double H5_RootGroup::getProjectionParameter5() const
{
	if (projectionParameter5) {
		return *projectionParameter5;
	}

	return 0;
}

void H5_RootGroup::setProjectionParameter5(const double value)
{
	if (!projectionParameter5)
	{
		projectionParameter5 = new double();
	}

	*projectionParameter5 = value;
}

bool H5_RootGroup::hasFalseNorthing() const
{
	return falseNorthing;
}

double H5_RootGroup::getFalseNorthing() const
{
	if (falseNorthing) {
		return *falseNorthing;
	}

	return 0;
}

void H5_RootGroup::setFalseNorthing(const double value)
{
	if (!falseNorthing)
	{
		falseNorthing = new double();
	}

	*falseNorthing = value;
}

bool H5_RootGroup::hasFalseEasting() const
{
	return falseEasting;
}

double H5_RootGroup::getFalseEasting() const
{
	if (falseEasting) {
		return *falseEasting;
	}
	
	return 0;
}

void H5_RootGroup::setFalseEasting(const double value)
{
	if (!falseEasting)
	{
		falseEasting = new double();
	}

	*falseEasting = value;
}

bool H5_RootGroup::hasEpoch() const
{
	return epoch;
}

std::string H5_RootGroup::getEpoch() const
{
	if (epoch) {
		return *epoch;
	}
	return "";
}

void H5_RootGroup::setEpoch(const std::string value)
{
	if (!epoch)
	{
		epoch = new std::string();
	}

	*epoch = value;
}

double H5_RootGroup::getWestBoundLongitude() const
{
	return westBoundLongitude;
}

void H5_RootGroup::setWestBoundLongitude(const double value) 
{
	westBoundLongitude = value;
}

double H5_RootGroup::getEastBoundLongitude() const
{
	return eastBoundLongitude;
}

void H5_RootGroup::setEastBoundLongitude(const double value)
{
	eastBoundLongitude = value;
}

double H5_RootGroup::getSouthBoundLatitude() const
{
	return southBoundLatitude;
}

void H5_RootGroup::setSouthBoundLatitude(const double value)
{
	southBoundLatitude = value;
}

double H5_RootGroup::getNorthBoundLatitude() const
{
	return northBoundLatitude;
}

void H5_RootGroup::setNorthBoundLatitude(const double value)
{
	northBoundLatitude = value;
}

bool H5_RootGroup::hasGeographicIdentifier() const
{
	return geographicIdentifier;
}

std::string H5_RootGroup::getGeographicIdentifier() const
{
	if (geographicIdentifier) {
		return *geographicIdentifier;
	}

	return "";
}

void H5_RootGroup::setGeographicIdentifier(const std::string value)
{
	if (!geographicIdentifier)
	{
		geographicIdentifier = new std::string();
	}

	*geographicIdentifier = value;
}

std::string H5_RootGroup::getMetadata() const
{
	return metadata;
}

void H5_RootGroup::setMetadata(const std::string value)
{
	metadata = value;
}

bool H5_RootGroup::hasVerticalCS() const
{
	return verticalCS;
}

int H5_RootGroup::getVerticalCS() const
{
	if (verticalCS) {
		return *verticalCS;
	}

	return 0;
}

void H5_RootGroup::setVerticalCS(const int value)
{
	if (!verticalCS)
	{
		verticalCS = new int();
	}

	*verticalCS = value;
}

bool H5_RootGroup::hasVerticalCoordinateBase() const
{
	return verticalCoordinateBase;
}

VerticalCoordinateBase H5_RootGroup::getVerticalCoordinateBase() const
{
	if (verticalCoordinateBase) {
		return *verticalCoordinateBase;
	}

	return VerticalCoordinateBase::none;
}

void H5_RootGroup::setVerticalCoordinateBase(const VerticalCoordinateBase value)
{
	if (!verticalCoordinateBase)
	{
		verticalCoordinateBase = new VerticalCoordinateBase();
	}

	*verticalCoordinateBase = value;
}

bool H5_RootGroup::hasVerticalDatumReference() const
{
	return verticalDatumReference;
}

VerticalDatumReference H5_RootGroup::getVerticalDatumReference() const
{
	if (verticalDatumReference) {
		return *verticalDatumReference;
	}

	return VerticalDatumReference::none;
}

void H5_RootGroup::setVerticalDatumReference(const VerticalDatumReference value)
{
	if (!verticalDatumReference)
	{
		verticalDatumReference = new VerticalDatumReference();
	}

	*verticalDatumReference = value;
}

bool H5_RootGroup::hasVerticalDatum() const
{
	return verticalDatum;
}

int H5_RootGroup::getVerticalDatum() const
{
	if (verticalDatum) {
		return *verticalDatum;
	}

	return 0;
}

void H5_RootGroup::setVerticalDatum(const int value)
{
	if (!verticalDatum)
	{
		verticalDatum = new int();
	}

	*verticalDatum = value;
}

bool H5_RootGroup::hasMetaFeatures() const
{
	return metaFeatures;
}

std::string H5_RootGroup::getMetaFeatures() const
{
	if (metaFeatures) {
		return *metaFeatures;
	}

	return "";
}

void H5_RootGroup::setMetaFeatures(const std::string value)
{
	if (!metaFeatures)
	{
		metaFeatures = new std::string();
	}

	*metaFeatures = value;
}

bool H5_RootGroup::Read(hid_t rootID)
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