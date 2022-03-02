#pragma once
#include "NamedType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class InformationAssociation :
	public NamedType
{
public:
	InformationAssociation();
	virtual ~InformationAssociation();

private:
	std::wstring superType = L"";
	std::list<std::wstring> subType; 
	Reference role[2];

public:
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetSuperType();
	void SetSuperType(std::wstring value);

	std::list<std::wstring>& GetSubTypePointer();

	Reference(&GetRolePointer())[2];
};