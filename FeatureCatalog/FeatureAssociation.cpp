#include "stdafx.h"
#include "FeatureAssociation.h"

FeatureAssociation::FeatureAssociation()
{

}

FeatureAssociation::~FeatureAssociation()
{

}

void FeatureAssociation::GetContents(pugi::xml_node& node)
{
	this->NamedType::GetContents(node);

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

std::list<std::wstring> &FeatureAssociation::GetSubTypePointer()
{
	return subType;
}

std::wstring& FeatureAssociation::GetSuperType()
{
	return superType;
}

Reference(&FeatureAssociation::GetRolePointer())[2]
{
	return role;
}