#pragma once
#include "XML_Item.h"
#include "SimpleAttribute.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class SimpleAttributes : public XML_Item
{
public:
	SimpleAttributes(void);
	virtual ~SimpleAttributes(void);

private:
	std::unordered_map<std::wstring, SimpleAttribute*> simpleAttribute;

public:
	void GetContents(pugi::xml_node& node);
	std::unordered_map<std::wstring, SimpleAttribute*>& GetSimpleAttributePointer();
};