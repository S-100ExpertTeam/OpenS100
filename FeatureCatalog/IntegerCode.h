#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

class IntegerCode :
	public Restriction_Item
{
public:
	IntegerCode();
	virtual ~IntegerCode();

public:
	void GetContents(pugi::xml_node& node);
};