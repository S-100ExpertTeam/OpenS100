#include "stdafx.h"
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

void XML_Item::Setvalue(std::wstring& value) 
{
	this->value = value;
}

const std::wstring& XML_Item::Getvalue()
{
	return value;
}

std::wstring* XML_Item::GetvaluePointer()
{
	return &value;
}

const std::wstring& XML_Item::GetReference()
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
			if (attr->Getname().compare(L"ref") == 0)
			{
				return attr->Getvalue();
			}
		}
	}

	return value;
}

void XML_Item::SetReference(std::wstring _value)
{
	value = _value;

	std::wstring refString = L"ref";
	XML_Attribute at;
	at.Setname(refString);
	at.Setvalue(_value);
	attributes.push_back(at);
}