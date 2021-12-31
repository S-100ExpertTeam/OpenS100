#include "stdafx.h"
#include "InformationType.h"

InformationType::InformationType()
{

}

InformationType::~InformationType()
{

}

void InformationType::GetContents(pugi::xml_node& node)
{
	this->S100ObjectType::GetContents(node);
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:superType"))
		{
			superType = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:subType"))
		{
			subType.push_back(pugi::as_wide(instruction.child_value()));
		}
	}
}

const std::wstring& InformationType::GetSuperType()
{
	return superType;
}

void InformationType::SetSuperType(std::wstring value)
{
	superType = value;
}

std::list<std::wstring>& InformationType::GetSubTypePointer()
{
	return subType;
}