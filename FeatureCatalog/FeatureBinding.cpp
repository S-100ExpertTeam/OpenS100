#include "stdafx.h"
#include "FeatureBinding.h"

FeatureBinding::FeatureBinding()
{

}

FeatureBinding::~FeatureBinding()
{

}

void FeatureBinding::GetContents(pugi::xml_node& node)
{
	if (node.attribute("roleType"))
	{
		XML_Attribute value;
		value.Setname("roleType");
		value.Setvalue((char*)node.attribute("roleType").value());
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
			association.GetContents(instruction);;
		}
		else if (!strcmp(instructionName, "S100FC:role"))
		{
			role.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:featureType"))
		{
			featureType.GetContents(instruction);
		}
	}
}

Multiplicity& FeatureBinding::GetMultiplicityPointer()
{
	return multiplicity;
}

Reference& FeatureBinding::GetAssociationPointer() 
{
	return association;
}

Reference& FeatureBinding::GetRolePointer() 
{
	return role;
}

Reference& FeatureBinding::GetFeatureTypePointer()
{
	return featureType;
}