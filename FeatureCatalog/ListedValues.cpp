#include "stdafx.h"
#include "ListedValues.h"

ListedValues::ListedValues()
{

}

ListedValues::~ListedValues()
{

}

void ListedValues::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:listedValue"))
		{
			ListedValue sa;
			sa.GetContents(instruction);
			listedValue.insert({ sa.GetCode().GetvalueInteger() ,sa });
			int i = 0;
		}
	}
}

std::unordered_map<int, ListedValue>& ListedValues::GetListedValuePointer()
{
	return listedValue;
}