#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

class Series : public XML_Item
{
public:
	Series();
	virtual ~Series();

private:
	std::string name = "";
	std::string issueIdentification = "";
	std::string page = "";

public:
	void GetContents(pugi::xml_node& node);

	void SetName();
	const std::string& GetName();

	void SetIssueIdentification(std::string value);
	const std::string& GetIssueIdentification();

	void SetPage(std::string valeu);
	const std::string& GetPage();
};