#include "stdafx.h"
#include "S100_AreaPlacement.h"

S100_AreaPlacement::S100_AreaPlacement()
{
}

S100_AreaPlacement::~S100_AreaPlacement()
{
}

void S100_AreaPlacement::GetContents(pugi::xml_node& node)
{
	if (!node)
		return;

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "placementMode"))
		{
			placementMode = instruction.child_value();
		}
	}
}

void S100_AreaPlacement::SetPlacementMode(const std::string& value)  { placementMode = value; }
void S100_AreaPlacement::SetPlacementMode(const std::wstring& value) { placementMode = toUtf8(value); }
std::string  S100_AreaPlacement::GetPlacementMode()  { return placementMode; }
std::wstring S100_AreaPlacement::GetPlacementModeW() { return toWide(placementMode); }
