#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

class DateTypeCode : public Restriction_Item
{
public:
	DateTypeCode();
	virtual ~DateTypeCode();

public:
	void GetContents(pugi::xml_node& node);
};