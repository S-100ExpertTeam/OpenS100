#pragma once
#include "ComplexAttribute.h"

#include <pugixml.hpp>

#include <unordered_map>
#include "StringUtil.h"

class ComplexAttributes :
	public XML_Item
{
public:
	ComplexAttributes(void);
	virtual ~ComplexAttributes(void);

private:
	std::unordered_map<std::string, ComplexAttribute*> complexAttribute;

public:
	void GetContents(pugi::xml_node& node);
	void SetComplexAttribute(std::string key, ComplexAttribute* value);
	void SetComplexAttribute(std::unordered_map<std::string, ComplexAttribute*> value);
	ComplexAttribute* GetComplexAttribute(std::string key);
	ComplexAttribute* GetComplexAttribute(std::wstring key) { return GetComplexAttribute(toUtf8(key)); }
	std::unordered_map<std::string, ComplexAttribute*>& GetComplexAttributePointer();
};
