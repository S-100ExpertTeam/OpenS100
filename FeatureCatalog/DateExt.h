#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

class DateExt : public XML_Item
{
public:
	DateExt();
	virtual ~DateExt();

private:
	std::string date = "";

public:
	void GetContents(pugi::xml_node& node);

	void SetDate(char* value);
	const std::string& GetDate();
};