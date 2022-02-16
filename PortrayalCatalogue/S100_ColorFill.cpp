#include "stdafx.h"
#include "S100_ColorFill.h"

S100_ColorFill::S100_ColorFill()
{
	SetType(1);
	color = NULL;
}

S100_ColorFill::~S100_ColorFill()
{
	if (color) delete color;
}

void S100_ColorFill::GetContents(pugi::xml_node node) 
{
	if (node==nullptr)
	{
		return;
	}

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"color"))
		{
			if (!color) color = new S100_Color();
			color->GetContents(instruction);
		}
		else
		{
			
		}
	}
}

void S100_ColorFill::SetColor(S100_Color* value) 
{
	color = value;
}

S100_Color* S100_ColorFill::GetColor() 
{
	return color;
}