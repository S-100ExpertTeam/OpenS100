#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

class OnlineFunctionCode :
	public Restriction_Item
{
public:
	OnlineFunctionCode();
	virtual ~OnlineFunctionCode();

public:
	void GetContents(pugi::xml_node& node);
};