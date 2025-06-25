#include "stdafx.h"
#include "S100_Pen.h"

S100_Pen::S100_Pen()
{

}

S100_Pen::~S100_Pen()
{

}

void S100_Pen::GetContents(pugi::xml_node& node)
{
	auto Attri = node.attribute("width");
	if (Attri)
	{
		width = pugi::as_wide(Attri.value());
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "color"))
		{
			color.GetContents(instruction);
		}
	}
}

void S100_Pen::SetWidth(std::wstring& value)
{
	width = value;
}

std::wstring S100_Pen::GetWidth() 
{
	return width;
}

void S100_Pen::SetColor(GraphicBasePackage::Color value) 
{
	color = value;
}

GraphicBasePackage::Color& S100_Pen::GetColor()
{
	return color;
}