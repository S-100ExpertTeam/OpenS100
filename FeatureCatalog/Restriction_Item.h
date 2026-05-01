#pragma once
#include "XML_Item.h"

#include <string>
#include <unordered_map>
#include "StringUtil.h"

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
	void SetPattern(std::wstring value) { SetPattern(toUtf8(value)); }
	const std::string& GetPattern();
	std::wstring GetPatternW() { return toWide(GetPattern()); }

	void SetMaxExclusive(std::string& value);
	void SetMaxExclusive(std::wstring value) { SetMaxExclusive(toUtf8(value)); }
	const std::string& GetMaxExclusive();
	std::wstring GetMaxExclusiveW() { return toWide(GetMaxExclusive()); }

	void SetValueString(std::string& value);
	void SetValueString(std::wstring value) { SetValueString(toUtf8(value)); }
	const std::string& GetValueString();
	std::wstring GetValueStringW() { return toWide(GetValueString()); }

	void SetValueInteger(int value);
	const int GetvalueInteger();
	void InsertValue(std::string value);
};
