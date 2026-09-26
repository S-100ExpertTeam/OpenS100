#pragma once
#include "XML_Attribute.h"

#include <list>
#include <string>
#include "StringUtil.h"

class XML_Item
{
public:
	XML_Item();
	virtual ~XML_Item();
	
private:
	std::list<XML_Attribute> attributes;
	std::string value = "";

public:
	virtual void GetContents() {};
	void SetAttributes(std::list<XML_Attribute>& value);
	void SetAttributes(XML_Attribute& value);
	std::list<XML_Attribute> Getattributes();
	std::list<XML_Attribute>& GetattributesPointer();

	void Setvalue(std::string& value);
	void Setvalue(const char* value);
	void Setvalue(std::wstring value);
	const std::string& Getvalue();
	std::wstring GetvalueW();
	std::string* GetvaluePointer();
	std::wstring GetvaluePointerW();

	const std::string& GetReference();
	std::wstring GetReferenceW();
	void SetReference(std::string _value);
	void SetReference(const char* value);
	void SetReference(std::wstring _value);
};
