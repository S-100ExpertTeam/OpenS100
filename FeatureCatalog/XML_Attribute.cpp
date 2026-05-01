#include "stdafx.h"
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