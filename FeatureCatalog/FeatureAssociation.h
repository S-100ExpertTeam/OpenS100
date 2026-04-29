#pragma once

#include "NamedType.h"

#include <pugixml.hpp>

class FeatureAssociation : public NamedType
{
public:
	FeatureAssociation();
	virtual ~FeatureAssociation();

private:
	std::string superType = "";
	std::list<std::string> subType;
	Reference role[2];

public:
	void GetContents(pugi::xml_node& node);
	std::list<std::string>& GetSubTypePointer();
	std::string& GetSuperType();
	Reference(&GetRolePointer())[2];
};