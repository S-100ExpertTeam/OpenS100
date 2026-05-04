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
			offset = instruction.child_value();
		}
		else if (!strcmp(instructionName, "placementMode"))
		{
			placementMode = instruction.child_value();
		}
	}
}

void S100_LinePlacement::SetOffset(const std::string& value)  { offset = value; }
void S100_LinePlacement::SetOffset(const std::wstring& value) { offset = toUtf8(value); }
std::string  S100_LinePlacement::GetOffset()  { return offset; }
std::wstring S100_LinePlacement::GetOffsetW() { return toWide(offset); }

void S100_LinePlacement::SetPlacementMode(const std::string& value)  { placementMode = value; }
void S100_LinePlacement::SetPlacementMode(const std::wstring& value) { placementMode = toUtf8(value); }
std::string  S100_LinePlacement::GetPlacementMode()  { return placementMode; }
std::wstring S100_LinePlacement::GetPlacementModeW() { return toWide(placementMode); }
