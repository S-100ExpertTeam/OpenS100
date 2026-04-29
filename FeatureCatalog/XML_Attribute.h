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
	void Setname(std::wstring values) { Setname(toUtf8(values)); }
	void Setname(char* value);
	const std::string& Getname();
	std::wstring GetnameW() { return toWide(Getname()); }

	void Setvalue(std::string& value);
	void Setvalue(std::wstring value) { Setvalue(toUtf8(value)); }
	void Setvalue(char* attribute);
	const std::string& Getvalue();
	std::wstring GetvalueW() { return toWide(Getvalue()); }
	std::string GetvalueString();
	std::wstring GetvalueStringW() { return toWide(GetvalueString()); }
};
