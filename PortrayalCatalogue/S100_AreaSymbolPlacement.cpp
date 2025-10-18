#include "stdafx.h"
#include "S100_AreaSymbolPlacement.h"

S100_AreaPlacementMode S100_AreaSymbolPlacement::GetPlacementMode() const
{
	return placementMode;
}

void S100_AreaSymbolPlacement::SetPlacementMode(const S100_AreaPlacementMode mode)
{
	placementMode = mode;
}

void S100_AreaSymbolPlacement::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "placementMode"))
		{
			placementMode = StringToS100AreaPlacementMode(instruction.child_value());
		}
	}
}