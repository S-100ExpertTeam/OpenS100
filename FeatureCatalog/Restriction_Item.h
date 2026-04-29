#pragma once
#include "XML_Item.h"

#include <string>
#include <unordered_map>

class Restriction_Item : public XML_Item
{
public:
	Restriction_Item();
	virtual ~Restriction_Item();

private:
	std::unordered_map<std::string, std::string> permittedValues;
	std::string pattern = "";
	std::string maxExclusive = "";
	std::string valueString = "";
	int valueInteger = 0;

public:
	void SetPermittedValues(std::unordered_map<std::string, std::string>& value);
	const std::unordered_map<std::string, std::string>& GetPermittedValues();

	void SetPattern(std::string& value);
	const std::string& GetPattern();

	void SetMaxExclusive(std::string& value);
	const std::string& GetMaxExclusive();

	void SetValueString(std::string& value);
	const std::string& GetValueString();

	void SetValueInteger(int value);
	const int GetvalueInteger();
	void InsertValue(std::string value);
};