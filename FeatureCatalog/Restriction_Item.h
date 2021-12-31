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
	std::unordered_map<std::wstring, std::wstring> permittedValues;
	std::wstring pattern = L"";
	std::wstring maxExclusive = L"";
	std::wstring valueString = L"";
	int valueInteger = 0;

public:
	void SetPermittedValues(std::unordered_map<std::wstring, std::wstring>& value);
	const std::unordered_map<std::wstring, std::wstring>& GetPermittedValues();

	void SetPattern(std::wstring& value);
	const std::wstring& GetPattern();

	void SetMaxExclusive(std::wstring& value);
	const std::wstring& GetMaxExclusive();

	void SetValueString(std::wstring& value);
	const std::wstring& GetValueString();

	void SetValueInteger(int value);
	const int GetvalueInteger();
	void InsertValue(std::wstring value);
};