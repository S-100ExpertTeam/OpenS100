#include "stdafx.h"
#include "S100_Element.h"

S100_Element::S100_Element()
{
	text = NULL;
	foreground = NULL;
	font = NULL;
}

S100_Element::~S100_Element()
{
	if (text) delete text;
	if (foreground) delete foreground;
	if (font) delete font;
}

void S100_Element::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "text"))
		{
			if (!text) text = new S100_Text();
			text->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "bodySize"))
		{
			bodySize = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "verticalOffset"))
		{
			verticalOffset = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "foreground"))
		{
			if (!foreground) foreground = new S100_Foreground();
			foreground->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "font"))
		{
			if (!font) font = new S100_Font();
			font->GetContents(instruction);
		}
	}
}

void S100_Element::SetText(S100_Text* value)
{
	text = value;
}

S100_Text* S100_Element::GetText()
{
	return text;
}

void S100_Element::SetBodySize(std::wstring& value) {
	bodySize = value;
}

std::wstring S100_Element::GetBodySize()
{
	return bodySize;
}

void S100_Element::SetVerticalOffset(std::wstring& value)
{
	verticalOffset = value;
}

std::wstring S100_Element::GetVerticalOffset()
{
	return verticalOffset;
}

void S100_Element::SetForground(S100_Foreground* value)
{
	foreground = value;
}

S100_Foreground* S100_Element::GetForground()
{
	return foreground;
}

void S100_Element::SetFont(S100_Font* value)
{
	font = value;
}

S100_Font* S100_Element::GetFont()
{
	return font;
}