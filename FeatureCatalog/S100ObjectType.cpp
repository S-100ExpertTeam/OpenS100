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
			
			informationBinding.push_back(ib);
		}
	}
}

std::list<InformationBinding*>& S100ObjectType::GetInformationBindingPointer()
{
	return informationBinding;
}