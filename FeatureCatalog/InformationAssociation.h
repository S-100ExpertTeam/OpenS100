#pragma once
#include "NamedType.h"

#include <pugixml.hpp>
#include "StringUtil.h"

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
	std::wstring GetSuperTypeW() { return toWide(GetSuperType()); }
	void SetSuperType(std::string value);
	void SetSuperType(std::wstring value) { SetSuperType(toUtf8(value)); }

	std::list<std::string>& GetSubTypePointer();

	Reference(&GetRolePointer())[2];
};
