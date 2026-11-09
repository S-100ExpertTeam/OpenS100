#include "stdafx.h"
#include "StringUtil.h"
#include "Restriction_Item.h"

Restriction_Item::Restriction_Item()
{

}
Restriction_Item::~Restriction_Item()
{

}

void Restriction_Item::InsertValue(std::string _value)
{
	permittedValues[_value] = _value;
	return;
}

void Restriction_Item::SetPermittedValues(std::unordered_map<std::string, std::string>& _value)
{
	permittedValues = _value;
}

const std::unordered_map<std::string, std::string>& Restriction_Item::GetPermittedValues()
{
	return permittedValues;
}

void Restriction_Item::SetPattern(std::string& _value)
{
	pattern = _value;
}

const std::string& Restriction_Item::GetPattern()
{
	return pattern;
}

void Restriction_Item::SetMaxExclusive(std::string& _value)
{
	maxExclusive = _value;
}

const std::string& Restriction_Item::GetMaxExclusive()
{
	return maxExclusive;
}

void Restriction_Item::SetValueString(std::string& _value)
{
	valueString = _value;
}

const std::string& Restriction_Item::GetValueString()
{
	return valueString;
}

void Restriction_Item::SetValueInteger(int _value)
{
	valueInteger = _value;
}

const int Restriction_Item::GetvalueInteger()
{
	return valueInteger;
}

void Restriction_Item::SetPattern(const char* _value)
{
	std::string s(_value ? _value : "");
	SetPattern(s);
}

void Restriction_Item::SetPattern(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetPattern(s);
}

std::wstring Restriction_Item::GetPatternW()
{
	return toWide(GetPattern());
}

void Restriction_Item::SetMaxExclusive(const char* _value)
{
	std::string s(_value ? _value : "");
	SetMaxExclusive(s);
}

void Restriction_Item::SetMaxExclusive(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetMaxExclusive(s);
}

std::wstring Restriction_Item::GetMaxExclusiveW()
{
	return toWide(GetMaxExclusive());
}

void Restriction_Item::SetValueString(const char* _value)
{
	std::string s(_value ? _value : "");
	SetValueString(s);
}

void Restriction_Item::SetValueString(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetValueString(s);
}

std::wstring Restriction_Item::GetValueStringW()
{
	return toWide(GetValueString());
}
