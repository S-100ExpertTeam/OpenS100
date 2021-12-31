#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class ValueList :
	public XML_Item
{
public:
	ValueList();
	virtual ~ValueList();

private:
	std::list<int> value;

public:
	void GetContents(pugi::xml_node& node);
	std::list<int>& GetValuePointer();
};