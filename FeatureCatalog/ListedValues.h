#pragma once
#include "XML_Item.h"
#include "ListedValue.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class ListedValues :
	public XML_Item
{
public:
	ListedValues();
	virtual ~ListedValues();

private:
	std::unordered_map<int, ListedValue> listedValue;

public:
	void GetContents(pugi::xml_node& node);
	std::unordered_map<int, ListedValue>& GetListedValuePointer();
};