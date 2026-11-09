#include "stdafx.h"
#include "InformationAssociations.h"

InformationAssociations::InformationAssociations()
{

}

InformationAssociations::~InformationAssociations()
{
	for (auto i = informationAssociation.begin(); i != informationAssociation.end(); i++)
	{
		delete i->second;
	}
}

void InformationAssociations::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_InformationAssociation"))
		{
			auto sa = new InformationAssociation();
			sa->GetContents(instruction);
			informationAssociation[sa->GetCode()] = sa;
			if (instruction.attribute("isAbstract"))
			{
				XML_Attribute _value;
				_value.Setname("isAbstract");
				_value.Setvalue((char*)instruction.attribute("isAbstract").value());
				SetAttributes(_value);
			}
		}
	}
}

std::unordered_map<std::string, InformationAssociation*>& InformationAssociations::GetInformationAssociationPointer()
{
	return informationAssociation;
}