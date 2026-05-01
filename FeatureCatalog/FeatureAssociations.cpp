#include "stdafx.h"
#include "FeatureAssociations.h"

FeatureAssociations::FeatureAssociations(void)
{

}

FeatureAssociations::~FeatureAssociations(void)
{
	for (auto i = featureAssociation.begin(); i != featureAssociation.end(); i++)
	{
		delete i->second;
	}
}

void FeatureAssociations::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_FeatureAssociation"))
		{
			auto sa = new FeatureAssociation();
			sa->GetContents(instruction);
			featureAssociation[sa->GetCodeAsWString()] = sa;

			if (instruction.attribute("isAbstract"))
			{
				XML_Attribute value;
				value.Setname("isAbstract");
				value.Setvalue((char*)instruction.attribute("isAbstract").value());
			}
		}
	}
}

std::unordered_map<std::string, FeatureAssociation*>& FeatureAssociations::GetFeatureAssociationPointer()
{
	return featureAssociation;
}