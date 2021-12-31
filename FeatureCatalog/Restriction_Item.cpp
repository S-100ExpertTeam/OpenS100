#include "stdafx.h"
#include "Restriction_Item.h"

Restriction_Item::Restriction_Item()
{

}
Restriction_Item::~Restriction_Item()
{

}

void Restriction_Item::InsertValue(std::wstring value)
{
	permittedValues[value] = value;
	return;
}

void Restriction_Item::SetPermittedValues(std::unordered_map<std::wstring, std::wstring>& value)
{
	permittedValues = value;
}

const std::unordered_map<std::wstring, std::wstring>& Restriction_Item::GetPermittedValues()
{
	return permittedValues;
}

void Restriction_Item::SetPattern(std::wstring& value)
{
	pattern = value;
}

const std::wstring& Restriction_Item::GetPattern()
{
	return pattern;
}

void Restriction_Item::SetMaxExclusive(std::wstring& value)
{
	maxExclusive = value;
}

const std::wstring& Restriction_Item::GetMaxExclusive()
{
	return maxExclusive;
}

void Restriction_Item::SetValueString(std::wstring& value)
{
	valueString = value;
}

const std::wstring& Restriction_Item::GetValueString()
{
	return valueString;
}

void Restriction_Item::SetValueInteger(int value)
{
	valueInteger = value;
}

const int Restriction_Item::GetvalueInteger()
{
	return valueInteger;
}