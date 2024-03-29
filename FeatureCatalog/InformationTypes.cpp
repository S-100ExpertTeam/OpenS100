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
			InsertInformationType(sa);
		}
	}
}

std::vector<InformationType*> InformationTypes::GetVecInformationType()
{
	return informationTypes;
}

void InformationTypes::ApplySuperType()
{
	for (auto itor = informationType.begin(); itor != informationType.end(); itor++)
	{
		InformationType* it = itor->second;
		SetAttributeFromSuperType(it);
		SetAssociationFromSuperType(it);
	}
}

bool InformationTypes::SetAttributeFromSuperType(InformationType* it)
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

			for (auto i = sit->GetAttributeBindingPointer().begin();
				i != sit->GetAttributeBindingPointer().end();
				i++)
			{
				auto ab = new AttributeBinding();
				*ab = *(*i);
				it->InsertAttributeBinding(ab);
			}

			return true;
		}
	}
	else
		return true;

	return false;
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
				for (auto i = sit->GetInformationBindingPointer().begin();
					i != sit->GetInformationBindingPointer().end();
					i++)
				{
					auto ib = new InformationBinding();
					*ib = *(*i);
					it->InsertInformationBinding(ib);
				}
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

bool InformationTypes::InsertInformationType(InformationType* value)
{
	auto key = value->GetCodeAsWString();
	if (informationType.find(key) == informationType.end())
	{
		informationTypes.push_back(value);
		informationType.insert({ key, value });
		return true;
	}

	delete value;
	return false;
}