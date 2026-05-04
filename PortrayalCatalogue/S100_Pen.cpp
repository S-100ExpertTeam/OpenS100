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
		width = Attri.value();
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

void S100_Pen::SetWidth(const std::string& value)  { width = value; }
void S100_Pen::SetWidth(const std::wstring& value) { width = toUtf8(value); }
std::string  S100_Pen::GetWidth()  { return width; }
std::wstring S100_Pen::GetWidthW() { return toWide(width); }

void S100_Pen::SetColor(GraphicBasePackage::Color value)
{
	color = value;
}

GraphicBasePackage::Color& S100_Pen::GetColor()
{
	return color;
}
