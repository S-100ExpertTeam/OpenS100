#include "stdafx.h"
#include "S100_LinePlacement.h"

S100_LinePlacement::S100_LinePlacement()
{

}

S100_LinePlacement::~S100_LinePlacement()
{

}

void S100_LinePlacement::GetContents(pugi::xml_node& node)
{
	if (!node)
		return;

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "offset"))
		{
			offset = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "placementMode"))
		{
			placementMode = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_LinePlacement::SetOffset(std::wstring& value)
{
	offset = value;
}

std::wstring S100_LinePlacement::GetOffset() 
{
	return offset;
}

void S100_LinePlacement::SetPlacementMode(std::wstring& value)
{
	placementMode = value;
}

std::wstring S100_LinePlacement::GetPlacementMode() 
{
	return placementMode;  
}