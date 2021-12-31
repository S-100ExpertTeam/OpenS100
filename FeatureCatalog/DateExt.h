#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class DateExt : public XML_Item
{
public:
	DateExt();
	virtual ~DateExt();

private:
	std::wstring date = L"";

public:
	void GetContents(pugi::xml_node& node);

	void SetDate(char* value);
	const std::wstring& GetDate();
};