#include "stdafx.h"
#include "NumericRange.h"

NumericRange::NumericRange()
{

}

NumericRange::~NumericRange()
{

}

void NumericRange::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100Base:lowerBound"))
		{
			lowerBound = std::stod(instruction.child_value());
		}

		else if (!strcmp(instructionName, "S100Base:upperBound"))
		{
			upperBound = std::stod(instruction.child_value());
		}

		else if (!strcmp(instructionName, "S100Base:intervalType"))
		{
			intervalType = stringToIntervalType(instruction.child_value());
		}
	}
}

void NumericRange::SetLowerBound(double value)
{
	lowerBound = value;
}

const double NumericRange::GetLowerBound() 
{
	return lowerBound.value();
}

bool NumericRange::hasLowerBound()
{
	return lowerBound.has_value();
}

void NumericRange::SetUpperBound(double value)
{
	upperBound = value;
}

const double NumericRange::GetUpperBound() 
{
	return upperBound.value();
}

bool NumericRange::hasUpperBound()
{
	return upperBound.has_value();
}

void NumericRange::SetIntervalType(IntervalType value)
{
	intervalType = value;
}

IntervalType& NumericRange::GetIntervalType() 
{
	return intervalType;
}