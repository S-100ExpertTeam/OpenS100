#pragma once
#include "XML_Item.h"
#include "Role.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class Roles : public XML_Item
{
public:
	Roles(void);
	virtual ~Roles(void);

private:
	std::unordered_map<std::wstring, Role*> role;

public:
	void GetContents(pugi::xml_node& node);
	std::unordered_map<std::wstring, Role*>& GetRolePointer();
};