#include "stdafx.h"
#include "InformationAssociations.h"

InformationAssociations::InformationAssociations()
{

}

InformationAssociations::~InformationAssociations()
{

}

void InformationAssociations::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_InformationAssociation"))
		{
			InformationAssociation sa;
			sa.GetContents(instruction);
			informationAssociation[sa.GetCodeAsWString()] = sa;
			if (instruction.attribute("isAbstract"))
			{
				XML_Attribute value;
				value.Setname("isAbstract");
				value.Setvalue((char*)instruction.attribute("isAbstract").value());
				SetAttributes(value);
			}
		}
	}
}

std::unordered_map<std::wstring, InformationAssociation>& InformationAssociations::GetInformationAssociationPointer()
{
	return informationAssociation;
}