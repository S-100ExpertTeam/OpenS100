#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class UnlimitedInteger :
	public XML_Item
{
public:
	UnlimitedInteger();
	virtual ~UnlimitedInteger();

public:
	void GetContents(pugi::xml_node& node);
};