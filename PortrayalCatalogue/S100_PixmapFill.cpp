#include "stdafx.h"
#include "S100_PixmapFill.h"

S100_PixmapFill::S100_PixmapFill()
{
	SetType(3);
}

S100_PixmapFill::~S100_PixmapFill()
{

}

void S100_PixmapFill::GetContents(pugi::xml_node node)
{
	if (!node)
		return;

	((S100_PatternFill*)this)->GetContents(node);

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"pixmap"))
		{
			pixmap.GetContents(instruction);
		}
	}
}

void S100_PixmapFill::SetPixmap(S100_Pixmap* value)
{
	pixmap = *value;
}

S100_Pixmap* S100_PixmapFill::GetPixmap() 
{
	return &pixmap;
}