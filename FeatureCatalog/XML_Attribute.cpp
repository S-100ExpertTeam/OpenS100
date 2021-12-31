#include "stdafx.h"
#include "XML_Attribute.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

XML_Attribute::XML_Attribute()
{
}

XML_Attribute::~XML_Attribute()
{
}

void XML_Attribute::Setname(std::wstring& values)
{
	name = value;
}

void XML_Attribute::Setname(char* value)
{
	name = pugi::as_wide(value);
}

 const std::wstring& XML_Attribute::Getname()
{
	return name;
}

void XML_Attribute::Setvalue(std::wstring& values)
{
	value = values;
}

void XML_Attribute::Setvalue(char* _value)
{
	value = pugi::as_wide(_value);
}

 const std::wstring& XML_Attribute::Getvalue()
{
	return value;
}

#pragma warning(disable:4996)
char* XML_Attribute::GetvalueString()
{
	std::string resultstring= std::string(value.begin(), value.end());
	char*  result = new char[resultstring.size()+1];
	strcpy(result, resultstring.c_str());
	return result;
}