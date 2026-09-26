#include "stdafx.h"
#include "StringUtil.h"
#include "Restriction_Item.h"

Restriction_Item::Restriction_Item()
{

}
Restriction_Item::~Restriction_Item()
{

}

void Restriction_Item::InsertValue(std::string value)
{
	permittedValues[value] = value;
	return;
}

void Restriction_Item::SetPermittedValues(std::unordered_map<std::string, std::string>& value)
{
	permittedValues = value;
}

const std::unordered_map<std::string, std::string>& Restriction_Item::GetPermittedValues()
{
	return permittedValues;
}

void Restriction_Item::SetPattern(std::string& value)
{
	pattern = value;
}

const std::string& Restriction_Item::GetPattern()
{
	return pattern;
}

void Restriction_Item::SetMaxExclusive(std::string& value)
{
	maxExclusive = value;
}

const std::string& Restriction_Item::GetMaxExclusive()
{
	return maxExclusive;
}

void Restriction_Item::SetValueString(std::string& value)
{
	valueString = value;
}

const std::string& Restriction_Item::GetValueString()
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

void Restriction_Item::SetPattern(const char* value)
{
	std::string s(value ? value : "");
	SetPattern(s);
}

void Restriction_Item::SetPattern(std::wstring value)
{
	std::string s = toUtf8(value);
	SetPattern(s);
}

std::wstring Restriction_Item::GetPatternW()
{
	return toWide(GetPattern());
}

void Restriction_Item::SetMaxExclusive(const char* value)
{
	std::string s(value ? value : "");
	SetMaxExclusive(s);
}

void Restriction_Item::SetMaxExclusive(std::wstring value)
{
	std::string s = toUtf8(value);
	SetMaxExclusive(s);
}

std::wstring Restriction_Item::GetMaxExclusiveW()
{
	return toWide(GetMaxExclusive());
}

void Restriction_Item::SetValueString(const char* value)
{
	std::string s(value ? value : "");
	SetValueString(s);
}

void Restriction_Item::SetValueString(std::wstring value)
{
	std::string s = toUtf8(value);
	SetValueString(s);
}

std::wstring Restriction_Item::GetValueStringW()
{
	return toWide(GetValueString());
}
