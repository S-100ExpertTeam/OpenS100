#include "stdafx.h"
#include "StringUtil.h"
#include "XML_Attribute.h"

#include <pugixml.hpp>

XML_Attribute::XML_Attribute()
{
}

XML_Attribute::~XML_Attribute()
{
}

void XML_Attribute::Setname(std::string& values)
{
	name = value;
}

void XML_Attribute::Setname(char* value)
{
	name = value;
}

 const std::string& XML_Attribute::Getname()
{
	return name;
}

void XML_Attribute::Setvalue(std::string& values)
{
	value = values;
}

void XML_Attribute::Setvalue(char* _value)
{
	value = _value;
}

 const std::string& XML_Attribute::Getvalue()
{
	return value;
}

#pragma warning(disable:4996)
std::string XML_Attribute::GetvalueString()
{
	std::string resultstring = std::string(value.begin(), value.end());
	return resultstring;
}

void XML_Attribute::Setname(const char* value)
{
	std::string s(value ? value : "");
	Setname(s);
}

void XML_Attribute::Setname(std::wstring values)
{
	std::string s = toUtf8(values);
	Setname(s);
}

std::wstring XML_Attribute::GetnameW()
{
	return toWide(Getname());
}

bool XML_Attribute::CompareName(const char* otherName)
{
	if (otherName == nullptr)
	{
		return false;
	}
	
	if (strcmp(name.c_str(), otherName) == 0)
	{
		return true;
	}

	return false;
}

void XML_Attribute::Setvalue(const char* value)
{
	std::string s(value ? value : "");
	Setvalue(s);
}

void XML_Attribute::Setvalue(std::wstring value)
{
	std::string s = toUtf8(value);
	Setvalue(s);
}

std::wstring XML_Attribute::GetvalueW()
{
	return toWide(Getvalue());
}

std::wstring XML_Attribute::GetvalueStringW()
{
	return toWide(GetvalueString());
}
