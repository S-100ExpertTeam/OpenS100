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
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:role"))
		{
			Reference reference;
			reference.GetContents(instruction);
			role[count] = reference;
			count++;
		}
		else if (!strcmp(instructionName, "S100FC:superType")) 
		{
			superType = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:subType"))
		{
			subType.push_back(instruction.child_value());
		}
	}
}

const std::string& InformationAssociation::GetSuperType()
{
	return superType;
}

void InformationAssociation::SetSuperType(std::string value)
{
	superType = value;
}

std::list<std::string>& InformationAssociation::GetSubTypePointer()
{
	return subType;
}

Reference(&InformationAssociation::GetRolePointer())[2]
{
	return role;
}