#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

class StringCode :
	public Restriction_Item
{
public:
	StringCode();
	virtual ~StringCode();

public:
	void GetContents(pugi::xml_node& node);
};