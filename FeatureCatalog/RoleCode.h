#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

class RoleCode :
	public Restriction_Item
{
public:
	RoleCode();
	virtual ~RoleCode();

public:
	void GetContents(pugi::xml_node& node);
};