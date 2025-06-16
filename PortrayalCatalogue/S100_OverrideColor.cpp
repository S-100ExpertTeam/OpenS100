#include "stdafx.h"
#include "S100_OverrideColor.h"

S100_OverrideColor::S100_OverrideColor()
{
	overrideColor = nullptr;
	color = nullptr;
}

S100_OverrideColor::~S100_OverrideColor()
{
	if (overrideColor)
		delete overrideColor, overrideColor = nullptr;

	if (color)
		delete color, color = nullptr;
}

void S100_OverrideColor::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "override"))
		{
			if (!overrideColor)
				overrideColor = new S100_Color();
			overrideColor->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "color"))
		{
			if (!color)
				color = new S100_Color();
			color->GetContents(instruction);
		}
	}
}

void S100_OverrideColor::SetOverrideColor(S100_Color* value)
{
	overrideColor = value;
}

S100_Color* S100_OverrideColor::GetOverrideColor()
{
	return overrideColor;
}

void S100_OverrideColor::SetColor(S100_Color* value)
{
	color = value;
}

S100_Color* S100_OverrideColor::GetColor()
{
	return color;
}