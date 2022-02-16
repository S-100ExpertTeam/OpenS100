#include "stdafx.h"
#include "S100_CompositeLineStyle.h"

S100_CompositeLineStyle::S100_CompositeLineStyle()
{
	SetType(3);
}

S100_CompositeLineStyle::~S100_CompositeLineStyle()
{
	for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
	{
		switch ((*itor)->GetType())
		{
		case 1:
			delete (S100_LineStyle*)*itor;
			break;
		case 2:
			delete (S100_LineStyleReference*)*itor;
			break;
		case 3:
			delete (S100_CompositeLineStyle*)*itor;
			break;
		}
	}
}

void S100_CompositeLineStyle::GetContents(pugi::xml_node& node) 
{
	if (!node) 
	{
		return;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		S100_LineStyle *lineStyle = NULL;

		if (!strcmp(instructionName, "lineStyle"))
		{
			lineStyle = new S100_LineStyle();
			lineStyle->GetContents(instruction);
			lineStyles.push_back(lineStyle);
		}
		else if (!strcmp(instructionName, "lineStyleReference"))
		{
			S100_LineStyleReference *lineStyleRef = new S100_LineStyleReference();
			lineStyleRef->GetContents(instruction);
			lineStyles.push_back(lineStyle);
		}
		else if (!strcmp(instructionName, "compositeLineStyle"))
		{
			S100_CompositeLineStyle *complexLineStyle = new S100_CompositeLineStyle();
			complexLineStyle->GetContents(instruction);
			lineStyles.push_back(lineStyle);
		}
	}
}

void S100_CompositeLineStyle::SetLineStyle(S100_LineStyle* value) 
{
	lineStyles.push_back(value);
}

void S100_CompositeLineStyle::SetLineStyles(std::vector<S100_LineStyle*> value) 
{
	lineStyles = value;
}

S100_LineStyle* S100_CompositeLineStyle::GetLineStyle(int index) 
{
	return lineStyles.at(index);
}

std::vector<S100_LineStyle*> S100_CompositeLineStyle::GetLineStyles()
{
	return lineStyles;
}