#pragma once
#include "XML_Item.h"
#include "CI_Organisation.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class Party :
	public XML_Item
{
public:
	Party();
	virtual ~Party();

private:
	CI_Organisation ci_Organisation;

public:
	void GetContents(pugi::xml_node& node);
	void SetCi_Organisation(CI_Organisation value);
	CI_Organisation& GetCi_Organisation();
};