#pragma once

#include "CV_SequenceType.h"
#include "CV_InterpolationMethod.h"

#include <hdf5.h>

class H5_FC_Attribute2
{
public:
	H5_FC_Attribute2();
	H5_FC_Attribute2(const H5_FC_Attribute2& other);
	virtual ~H5_FC_Attribute2();

public:
	CV_SequenceType sequencingRuleType = CV_SequenceType::none;
	std::string sequencingRuleScanDirection;
	CV_InterpolationMethod interpolationMethod = CV_InterpolationMethod::none;

public:
	virtual H5_FC_Attribute2 operator=(const H5_FC_Attribute2& other);

public:
	CV_SequenceType getSequencingRuleType() const;
	void setSequencingRuleType(const CV_SequenceType value);

	std::string getSequencingRuleScanDirection() const;
	void setSequencingRuleScanDirection(const std::string& value);

	CV_InterpolationMethod getInterpolationMethod() const;
	void setInterpolationMethod(const CV_InterpolationMethod value);

public:
	bool Read(hid_t groupID);
};

