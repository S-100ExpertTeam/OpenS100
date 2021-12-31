#include "stdafx.h"
#include "InformationAssociation.h"

InformationAssociation::InformationAssociation()
{

}

InformationAssociation::~InformationAssociation()
{

}

void InformationAssociation::GetContents(pugi::xml_node& node)
{
	((NamedType*)this)->NamedType::GetContents(node);
	int count = 0;
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:role"))
		{
			Reference reference;
			reference.GetContents(instruction);
			role[count] = reference;
			count++;
		}
		else if (!strcmp(instructionName, "S100FC:superType")) 
		{
			superType = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:subType"))
		{
			subType.push_back(pugi::as_wide(instruction.child_value()));
		}
	}
}

const std::wstring& InformationAssociation::GetSuperType()
{
	return superType;
}

void InformationAssociation::SetSuperType(std::wstring value)
{
	superType = value;
}

std::list<std::wstring>& InformationAssociation::GetSubTypePointer()
{
	return subType;
}

Reference(&InformationAssociation::GetRolePointer())[2]
{
	return role;
}