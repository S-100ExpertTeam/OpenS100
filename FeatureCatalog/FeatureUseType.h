#pragma once
#include "Restriction_Item.h"

#include <pugixml.hpp>

class FeatureUseType :
	public Restriction_Item
{
public:
	FeatureUseType();
	virtual ~FeatureUseType();

public:
	void GetContents(pugi::xml_node& node);
};