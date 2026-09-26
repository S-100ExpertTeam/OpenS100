#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>
#include "StringUtil.h"

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
	std::wstring GetNameW();

	void SetIssueIdentification(std::string value);
	void SetIssueIdentification(const char* value);
	void SetIssueIdentification(std::wstring value);
	const std::string& GetIssueIdentification();
	std::wstring GetIssueIdentificationW();

	void SetPage(std::string valeu);
	void SetPage(const char* value);
	void SetPage(std::wstring valeu);
	const std::string& GetPage();
	std::wstring GetPageW();
};
