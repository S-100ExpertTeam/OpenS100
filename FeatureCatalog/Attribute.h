#pragma once
#include "Item.h"

#include <pugixml.hpp>

// S100_FC_Attribute
class Attribute : public Item
{
public:
	Attribute();
	virtual ~Attribute();

public:
	void GetContents(pugi::xml_node& node);

	virtual bool IsSimple() { return false; };
	virtual bool IsComplex() { return false; };
};