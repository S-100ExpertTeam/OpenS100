#include "StdAfx.h"
#include "SimpleAttributes.h"

SimpleAttributes::SimpleAttributes(void)
{

}

SimpleAttributes::~SimpleAttributes(void)
{
	for (auto i = simpleAttribute.begin(); i != simpleAttribute.end(); i++)
	{
		delete i->second;
	}
}

void SimpleAttributes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_SimpleAttribute"))
		{
			auto sa = new SimpleAttribute();
			sa->GetContents(instruction);
			simpleAttribute.insert({ sa->GetCodeAsWString(), sa });
		}
	}
}

std::unordered_map<std::wstring, SimpleAttribute*>& SimpleAttributes::GetSimpleAttributePointer()
{
	return simpleAttribute;
}
