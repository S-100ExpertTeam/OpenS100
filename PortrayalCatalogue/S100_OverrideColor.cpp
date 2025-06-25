#include "stdafx.h"
#include "S100_OverrideColor.h"

S100_OverrideColor::S100_OverrideColor()
{

}

S100_OverrideColor::~S100_OverrideColor()
{

}

void S100_OverrideColor::GetContents(pugi::xml_node node) 
{
	if (node==nullptr)
	{
		return;
	}


	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"override"))
		{
			overrideColor.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "color"))
		{
			color.GetContents(instruction);
		}
	}
}

void S100_OverrideColor::SetOverrideColor(S100_Color* value) 
{
	overrideColor = *value;
}

S100_Color* S100_OverrideColor::GetOverrideColor() 
{
	return &overrideColor;
}

void S100_OverrideColor::SetColor(S100_Color* value) 
{
	color = *value;
}

S100_Color* S100_OverrideColor::GetColor() 
{
	return &color;
}