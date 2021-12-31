#pragma once
#include "Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class Role :
	public Item
{
public:
	Role();
	virtual ~Role();

public:
	void GetContents(pugi::xml_node& node);
};