#pragma once
#include "XML_Item.h"
#include "NumericRange.h"

#include <pugixml.hpp>
#include "StringUtil.h"

class AttributeConstraints :
	public XML_Item
{
public:
	AttributeConstraints();
	virtual ~AttributeConstraints();

public:
	int stringLength = 0;
	std::string textPattern = "";
	NumericRange range;
	int precision = 0;

public:
	void GetContents(pugi::xml_node& node);

	int GetStringLength();
	void SetStringLength(int value);
	
	const std::string& GetTextPattern();
	std::wstring GetTextPatternW();
	void GetTextPattern(std::string value);
	void GetTextPattern(std::wstring value);

	NumericRange& GetRangePointer();

	int GetPrecision();
	void SetPrecision(int value);

};
