#pragma once
#include "XML_Item.h"
#include "IntervalType.h"

#include <pugixml.hpp>

#include <optional>

class NumericRange
{
public:
	NumericRange();
	virtual ~NumericRange();

private:
	std::optional<double> lowerBound;
	std::optional<double> upperBound;
	IntervalType intervalType = IntervalType::none;

public:
	void GetContents(pugi::xml_node& node);

	void SetLowerBound(double value);
	const double GetLowerBound();
	bool hasLowerBound();

	void SetUpperBound(double value);
	const double GetUpperBound();
	bool hasUpperBound();

	void SetIntervalType(IntervalType value);
	IntervalType& GetIntervalType();
};