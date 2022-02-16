#include "stdafx.h"
#include "S100_LineStyleReference.h"

S100_LineStyleReference::S100_LineStyleReference()
{
	SetType(2);
}

S100_LineStyleReference::~S100_LineStyleReference()
{

}

void S100_LineStyleReference::GetContents(pugi::xml_node& node)
{
	if (!node) 
	{
		return;
	}


	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "reference"))
		{
			reference = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_LineStyleReference::SetReference(std::wstring& value)
{
	reference = value;
}
std::wstring S100_LineStyleReference::GetReference() 
{
	return reference;
}