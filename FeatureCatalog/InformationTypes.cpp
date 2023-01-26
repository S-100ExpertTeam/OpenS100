#include "stdafx.h"
#include "InformationTypes.h"

InformationTypes::InformationTypes(void)
{

}

InformationTypes::~InformationTypes(void)
{
	for (auto i = informationType.begin(); i != informationType.end(); i++)
	{
		delete i->second;
	}
}

void InformationTypes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_InformationType"))
		{
			auto sa = new InformationType();
			sa->GetContents(instruction);
			AddInformationType(sa);
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
			InformationType* sit = itor->second;
			if (SetAssociationFromSuperType(sit))
			{
				it->GetInformationBindingPointer().insert(
					it->GetInformationBindingPointer().end(),
					sit->GetInformationBindingPointer().begin(), 
					sit->GetInformationBindingPointer().end());
			}
			return true;
		}

	}
	else
		return true;

	return false;

}

std::unordered_map<std::wstring, InformationType*>& InformationTypes::GetInformationTypePointer()
{
	return informationType;
}

void InformationTypes::AddInformationType(InformationType* item)
{
	if (informationType.end() == informationType.find(item->GetCodeAsWString()))
	{
		informationType.insert({ item->GetCodeAsWString(), item });
		informationTypes.push_back(item);
	}
}