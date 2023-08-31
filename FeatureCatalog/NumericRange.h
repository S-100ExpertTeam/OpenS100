#pragma once
#include "XML_Item.h"
#include "IntervalType.h"

#include <pugixml.hpp>

class NumericRange
{
public:
	NumericRange();
	virtual ~NumericRange();

private:
	double lowerBound = 0.;
	double upperBound = 0.;
	IntervalType intervalType;

public:
	void GetContents(pugi::xml_node& node);

	void SetLowerBound(double value);
	const double GetLowerBound();

	void SetUpperBound(double value);
	const double GetUpperBound();

	void SetIntervalType(IntervalType value);
	IntervalType& GetIntervalType();
};