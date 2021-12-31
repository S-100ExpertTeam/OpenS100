#include "stdafx.h"
#include "InformationBinding.h"

InformationBinding::InformationBinding()
{

}

InformationBinding::~InformationBinding()
{

}

void InformationBinding::GetContents(pugi::xml_node& node)
{
	auto attribute = node.attribute("roleType");
	if (attribute)
	{
		XML_Attribute value;
		value.Setname("roleType");
		value.Setvalue((char*)attribute.value());

		SetAttributes(value);
	}
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:multiplicity"))
		{
			multiplicity.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:association"))
		{
			association.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:role"))
		{
			role.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:informationType"))
		{
			informationType.GetContents(instruction);
		}
	}
}

Multiplicity& InformationBinding::GetMultiplicityPointer()
{
	return multiplicity;
}

Reference& InformationBinding::GetAssociationPointer()
{
	return association;
}

Reference& InformationBinding::GetRolePointer()
{
	return role;
}

Reference& InformationBinding::GetInformationTypePointer()
{
	return informationType;
}