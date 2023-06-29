#include "stdafx.h"
#include "H5_FC_Attribute2.h"
#include "HDF5Wrapper.h"

H5_FC_Attribute2::H5_FC_Attribute2()
{

}

H5_FC_Attribute2::~H5_FC_Attribute2()
{

}

CV_SequenceType H5_FC_Attribute2::getSequencingRuleType() const
{
	return sequencingRuleType;
}

void H5_FC_Attribute2::setSequencingRuleType(const CV_SequenceType value)
{
	sequencingRuleType = value;
}

std::string H5_FC_Attribute2::getSequencingRuleScanDirection() const
{
	return sequencingRuleScanDirection;
}

void H5_FC_Attribute2::setSequencingRuleScanDirection(const std::string& value)
{
	sequencingRuleScanDirection = value;
}

CV_InterpolationMethod H5_FC_Attribute2::getInterpolationMethod() const
{
	return interpolationMethod;
}

void H5_FC_Attribute2::setInterpolationMethod(const CV_InterpolationMethod value)
{
	interpolationMethod = value;
}

bool H5_FC_Attribute2::Read(hid_t groupID)
{
	int iSequencingRuleType = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "type", iSequencingRuleType)) {
		setSequencingRuleType((CV_SequenceType)iSequencingRuleType);
	}

	std::string strSequencingRuleScanDirection = 0;
	if (HDF5Wrapper::ReadMetadataAnyStringAttribute(groupID, "scanDirection", strSequencingRuleScanDirection)) {
		setSequencingRuleScanDirection(strSequencingRuleScanDirection);
	}

	int iInterpolationMethod = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(groupID, "interpolationType", iInterpolationMethod)) {
		setInterpolationMethod((CV_InterpolationMethod)iInterpolationMethod);
	}

	return true;
}