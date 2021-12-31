#pragma once
#include "ComplexAttribute.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <unordered_map>

class ComplexAttributes :
	public XML_Item
{
public:
	ComplexAttributes(void);
	virtual ~ComplexAttributes(void);

private:
	std::unordered_map<std::wstring, ComplexAttribute> complexAttribute;

public:
	void GetContents(pugi::xml_node& node);
	void SetComplexAttribute(std::wstring key, ComplexAttribute value);
	void SetComplexAttribute(std::unordered_map<std::wstring, ComplexAttribute> value);
	ComplexAttribute GetComplexAttribute(std::wstring key);
	std::unordered_map<std::wstring, ComplexAttribute>& GetComplexAttributePointer();
};