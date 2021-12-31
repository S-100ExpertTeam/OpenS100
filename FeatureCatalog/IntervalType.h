#pragma once
#include "Restriction_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class IntervalType :
	public Restriction_Item
{
public:
	IntervalType();
	virtual ~IntervalType();

public:
	void GetContents(pugi::xml_node& node);
};