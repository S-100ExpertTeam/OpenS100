#include "StdAfx.h"
#include "InformationTypes.h"

InformationTypes::InformationTypes(void)
{

}

InformationTypes::~InformationTypes(void)
{

}

void InformationTypes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_InformationType"))
		{
			InformationType sa;
			sa.GetContents(instruction);
			informationType[sa.GetCodeAsWString()] = sa;
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

bool InformationTypes::SetAssociationFromSuperType(InformationType* it)
{
	if (it->GetSuperType().size() > 0)
	{
		auto itor = informationType.find(it->GetSuperType());
		if (itor == informationType.end())
		{
			return false;
		}
		else
		{
			InformationType* sit = &itor->second;
			if (SetAssociationFromSuperType(sit))
			{
				it->GetInformationBindingPointer().insert(sit->GetInformationBindingPointer().begin(), sit->GetInformationBindingPointer().end());
			}
			return true;
		}

	}
	else
		return true;

	return false;

}

std::unordered_map<std::wstring, InformationType>& InformationTypes::GetInformationTypePointer()
{
	return informationType;
}
