#include "stdafx.h"
#include "StringUtil.h"
#include "XML_Item.h"

XML_Item::XML_Item()
{

}

XML_Item::~XML_Item()
{

}

void XML_Item::SetAttributes(std::list<XML_Attribute>& value)
{
	attributes = value;
}

void XML_Item::SetAttributes(XML_Attribute& value)
{
	attributes.push_back(value);
}

std::list<XML_Attribute> XML_Item::Getattributes()
{
	return attributes;
}

std::list<XML_Attribute>& XML_Item::GetattributesPointer()
{
	return attributes;
}

void XML_Item::Setvalue(std::string& value) 
{
	this->value = value;
}

const std::string& XML_Item::Getvalue()
{
	return value;
}

std::string* XML_Item::GetvaluePointer()
{
	return &value;
}

const std::string& XML_Item::GetReference()
{
	if (value.size() > 0)
		return value;
	else
	{
		for (auto itor = attributes.begin();
			itor != attributes.end();
			itor++)
		{
			XML_Attribute* attr = &(*itor);
			if (attr->Getname().compare("ref") == 0)
			{
				return attr->Getvalue();
			}
		}
	}

	return value;
}

void XML_Item::SetReference(std::string _value)
{
	value = _value;

	std::string refString = "ref";
	XML_Attribute at;
	at.Setname(refString);
	at.Setvalue(_value);
	attributes.push_back(at);
}

void XML_Item::Setvalue(const char* value)
{
	std::string s(value ? value : "");
	Setvalue(s);
}

void XML_Item::Setvalue(std::wstring value)
{
	std::string s = toUtf8(value);
	Setvalue(s);
}

std::wstring XML_Item::GetvalueW()
{
	return toWide(Getvalue());
}

std::wstring XML_Item::GetvaluePointerW()
{
	return toWide(*GetvaluePointer());
}

std::wstring XML_Item::GetReferenceW()
{
	return toWide(GetReference());
}

void XML_Item::SetReference(const char* value)
{
	std::string s(value ? value : "");
	SetReference(s);
}

void XML_Item::SetReference(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetReference(s);
}
