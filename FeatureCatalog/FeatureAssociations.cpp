#include "StdAfx.h"
#include "FeatureAssociations.h"

FeatureAssociations::FeatureAssociations(void)
{

}

FeatureAssociations::~FeatureAssociations(void)
{

}

void FeatureAssociations::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_FeatureAssociation"))
		{
			FeatureAssociation sa;
			sa.GetContents(instruction);
			featureAssociation[sa.GetCodeAsWString()] = sa;

			if (instruction.attribute("isAbstract"))
			{
				XML_Attribute value;
				value.Setname("isAbstract");
				value.Setvalue((char*)instruction.attribute("isAbstract").value());
			}
		}
	}
}

std::unordered_map<std::wstring, FeatureAssociation>& FeatureAssociations::GetFeatureAssociationPointer()
{
	return featureAssociation;
}