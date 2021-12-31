#pragma once
#include "Restriction_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class PresentationFormCode : public Restriction_Item
{
public:
	PresentationFormCode();
	virtual ~PresentationFormCode();

public:
	void GetContents(pugi::xml_node& node);
};