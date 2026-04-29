#pragma once
#include "NamedType.h"

#include <pugixml.hpp>

class InformationAssociation :
	public NamedType
{
public:
	InformationAssociation();
	virtual ~InformationAssociation();

private:
	std::string superType = "";
	std::list<std::string> subType; 
	Reference role[2];

public:
	void GetContents(pugi::xml_node& node);

	const std::string& GetSuperType();
	void SetSuperType(std::string value);

	std::list<std::string>& GetSubTypePointer();

	Reference(&GetRolePointer())[2];
};