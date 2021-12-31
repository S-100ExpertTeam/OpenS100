#pragma once

#include <string>

class XML_Attribute
{
public:
	XML_Attribute();
	virtual ~XML_Attribute();

private:
	std::wstring name = L"";
	std::wstring value = L"";

public:
	void Setname(std::wstring& values);
	void Setname(char* value);
	const std::wstring& Getname();

	void Setvalue(std::wstring& value);
	void Setvalue(char* attribute);
	const std::wstring& Getvalue();
	char* GetvalueString();
};