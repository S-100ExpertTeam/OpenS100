#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class Series : public XML_Item
{
public:
	Series();
	virtual ~Series();

private:
	std::wstring name = L"";
	std::wstring issueIdentification = L"";
	std::wstring page = L"";

public:
	void GetContents(pugi::xml_node& node);

	void SetName();
	const std::wstring& GetName();

	void SetIssueIdentification(std::wstring value);
	const std::wstring& GetIssueIdentification();

	void SetPage(std::wstring valeu);
	const std::wstring& GetPage();
};