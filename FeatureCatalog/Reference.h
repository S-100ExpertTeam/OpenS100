#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class Reference :
	public XML_Item
{
public:
	Reference();
	virtual ~Reference();

public:
	void GetContents(pugi::xml_node& node);
};