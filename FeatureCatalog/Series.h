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
	std::wstring GetNameW() { return toWide(GetName()); }

	void SetIssueIdentification(std::string value);
	void SetIssueIdentification(std::wstring value) { SetIssueIdentification(toUtf8(value)); }
	const std::string& GetIssueIdentification();
	std::wstring GetIssueIdentificationW() { return toWide(GetIssueIdentification()); }

	void SetPage(std::string valeu);
	void SetPage(std::wstring valeu) { SetPage(toUtf8(valeu)); }
	const std::string& GetPage();
	std::wstring GetPageW() { return toWide(GetPage()); }
};
