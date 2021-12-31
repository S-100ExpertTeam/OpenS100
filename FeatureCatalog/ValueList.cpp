#include "stdafx.h"
#include "ValueList.h"

ValueList::ValueList()
{

}

ValueList::~ValueList()
{

}

void ValueList::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:value"))
		{
			int Value = std::stoi(instruction.child_value());
			value.push_back(Value);
		}
	}
}

std::list<int>& ValueList::GetValuePointer()
{
	return value;
}