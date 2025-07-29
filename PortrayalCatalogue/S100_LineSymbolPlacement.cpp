#include "stdafx.h"
#include "S100_LineSymbolPlacement.h"

double S100_LineSymbolPlacement::GetOffset() const
{
	return offset;
}

void S100_LineSymbolPlacement::SetOffset(double value)
{
	offset = value;
}

S100_LinePlacementMode S100_LineSymbolPlacement::GetPlacementMode() const
{
	return placementMode;
}

void S100_LineSymbolPlacement::SetPlacementMode(S100_LinePlacementMode value)
{
	placementMode = value;
}

std::optional<bool> S100_LineSymbolPlacement::GetVisibleParts() const
{
	return visibleParts;
}

void S100_LineSymbolPlacement::SetVisibleParts(bool value)
{
	visibleParts = value;
}

void S100_LineSymbolPlacement::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "offset"))
		{
			//offset = instruction.child_value();
			offset = instruction.text().as_double();
		}
		else if (!strcmp(instructionName, "placementMode"))
		{
			//placementMode = pugi::as_wide(instruction.child_value());
			placementMode = StringToLinePlacementMode(instruction.child_value());
		}
	}
}