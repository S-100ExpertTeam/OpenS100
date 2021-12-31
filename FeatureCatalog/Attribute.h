#pragma once
#include "Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

// S100_FC_Attribute
class Attribute : public Item
{
public:
	Attribute();
	virtual ~Attribute();

public:
	void GetContents(pugi::xml_node& node);
};