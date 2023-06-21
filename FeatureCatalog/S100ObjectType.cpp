#include "stdafx.h"
#include "S100ObjectType.h"

S100ObjectType::S100ObjectType()
{

}

S100ObjectType::~S100ObjectType()
{
	for (auto i = informationBinding.begin(); i != informationBinding.end(); i++)
	{
		delete *i;
	}
}

void S100ObjectType::GetContents(pugi::xml_node& node)
{
	this->NamedType::GetContents(node);

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:informationBinding"))
		{
			auto ib = new InformationBinding();
			ib->GetContents(instruction);
			if (auto targetIB = CanMerged(*ib))
			{
				targetIB->AppendInformationBinding(*ib);
				delete ib;
				ib = nullptr;
			}
			else
			{
				InsertInformationBinding(ib);
			}
		}
	}
}

std::list<InformationBinding*>& S100ObjectType::GetInformationBindingPointer()
{
	return informationBinding;
}

void S100ObjectType::InsertInformationBinding(InformationBinding* value)
{
	for (auto i = informationBinding.begin(); i != informationBinding.end(); i++) {
		if ((*i)->IsSameAssociation(*value)) {
			delete value;
			return;
		}
	}

	informationBinding.push_back(value);
}

InformationBinding* S100ObjectType::CanMerged(InformationBinding& ib)
{
	for (auto i = informationBinding.begin(); i != informationBinding.end(); i++)
	{
		auto currentIB = *i;
		if (currentIB->IsSameAssociation(ib))
		{
			return currentIB;
		}
	}

	return nullptr;
}