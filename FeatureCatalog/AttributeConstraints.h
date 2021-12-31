#pragma once
#include "XML_Item.h"
#include "NumericRange.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class AttributeConstraints :
	public XML_Item
{
public:
	AttributeConstraints();
	virtual ~AttributeConstraints();

public:
	int stringLength = 0;
	std::wstring textPattern = L"";
	NumericRange range;
	int precision = 0;

public:
	void GetContents(pugi::xml_node& node);

	int GetStringLength();
	void SetStringLength(int value);
	
	const std::wstring& GetTextPattern();
	void GetTextPattern(std::wstring value);

	NumericRange& GetRangePointer();

	int GetPrecision();
	void SetPrecision(int value);

};