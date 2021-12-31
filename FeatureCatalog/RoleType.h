#pragma once
#include "Restriction_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class RoleType :
	public Restriction_Item
{
public:
	RoleType();
	virtual ~RoleType();

public:
	void GetContents(pugi::xml_node& node);
};