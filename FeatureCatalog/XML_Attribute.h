#pragma once

#include <string>
#include "StringUtil.h"

class XML_Attribute
{
public:
	XML_Attribute();
	virtual ~XML_Attribute();

private:
	std::string name = "";
	std::string value = "";

public:
	void Setname(std::string& values);
	void Setname(const char* value);
	void Setname(std::wstring values);
	void Setname(char* value);
	const std::string& Getname();
	std::wstring GetnameW();
	bool CompareName(const char* otherName);

	void Setvalue(std::string& value);
	void Setvalue(const char* value);
	void Setvalue(std::wstring value);
	void Setvalue(char* attribute);
	const std::string& Getvalue();
	std::wstring GetvalueW();
	std::string GetvalueString();
	std::wstring GetvalueStringW();
};
