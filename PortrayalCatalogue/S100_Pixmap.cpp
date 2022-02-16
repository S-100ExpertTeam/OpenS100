#include "stdafx.h"
#include "S100_Pixmap.h"

S100_Pixmap::S100_Pixmap()
{

}

S100_Pixmap::~S100_Pixmap()
{

}

void S100_Pixmap::GetContents(pugi::xml_node node) 
{
	if (node==nullptr)
	{
		return;
	}
	
	for (auto instruction= node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"overrideAll"))
		{
			overrideAll.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "override"))
		{
			overrideColor.GetContents(instruction);
		}
	}
}

void S100_Pixmap::SetReference(std::wstring& value)
{
	reference = value;
}

std::wstring S100_Pixmap::GetReference() 
{
	return reference;
}

void S100_Pixmap::SetOverrideAll(S100_Color* value) 
{
	overrideAll = *value;
}

S100_Color* S100_Pixmap::GetOverrideAll() 
{
	return &overrideAll;
}

void S100_Pixmap::SetOverrideColor(S100_OverrideColor* value) 
{
	overrideColor = *value;
}

S100_OverrideColor* S100_Pixmap::GetOverrideColor() 
{
	return &overrideColor;
}