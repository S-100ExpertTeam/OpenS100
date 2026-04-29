#pragma once

#include <string>

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
	void Setname(char* value);
	const std::string& Getname();

	void Setvalue(std::string& value);
	void Setvalue(char* attribute);
	const std::string& Getvalue();
	std::string GetvalueString();
};