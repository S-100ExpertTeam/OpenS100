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
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:superType"))
		{
			superType = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:subType"))
		{
			subType.push_back(instruction.child_value());
		}
	}
}

const std::string& InformationType::GetSuperType()
{
	return superType;
}

void InformationType::SetSuperType(std::string value)
{
	superType = value;
}

std::list<std::string>& InformationType::GetSubTypePointer()
{
	return subType;
}