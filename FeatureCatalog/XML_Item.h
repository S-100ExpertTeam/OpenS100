#pragma once
#include "XML_Attribute.h"

#include <list>
#include <string>

class XML_Item
{
public:
	XML_Item();
	virtual ~XML_Item();
	
private:
	std::list<XML_Attribute> attributes;
	std::wstring value = L"";

public:
	virtual void GetContents() {};
	void SetAttributes(std::list<XML_Attribute>& value);
	void SetAttributes(XML_Attribute& value);
	std::list<XML_Attribute> Getattributes();
	std::list<XML_Attribute>& GetattributesPointer();

	void Setvalue(std::wstring& value);
	const std::wstring& Getvalue();
	std::wstring* GetvaluePointer();

	const std::wstring& GetReference();
	void SetReference(std::wstring _value);
};