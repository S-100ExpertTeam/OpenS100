#pragma once
#include "XML_Item.h"
#include "ContactInfo.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class CI_Organisation :
	public XML_Item
{
public:
	CI_Organisation();
	virtual ~CI_Organisation();

private:
	std::string name = "";
	ContactInfo contactinfo;

public:
	void GetContents(pugi::xml_node& node);

	void SetName(char* value);
	const std::string GetName();

	void SetContactinfo(ContactInfo& value);
	const ContactInfo& GetContactinfo();
};