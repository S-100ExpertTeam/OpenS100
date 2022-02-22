#include "stdafx.h"
#include "S100_Hatch.h"

S100_Hatch::S100_Hatch()
{

}

S100_Hatch::~S100_Hatch()
{

}

void S100_Hatch::GetContents(pugi::xml_node node)
{
	if (node==nullptr)
	{
		return;
	}


	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"lineStyle"))
		{
			lineStyle.GetContents(instruction);
		}
		else if (!strcmp(instructionName,"lineStyleReference")) 
		{
			lineStyleReference.GetContents(instruction);
		}
		else if (!strcmp(instructionName,"compositeLineStyle"))
		{
			compositeLineStyle.GetContents(instruction);
		}
		else if (!strcmp(instructionName,"direction"))
		{
			direction.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "distance")) 
		{
			distance = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_Hatch::SetLineStyle(S100_LineStyle value) 
{
	lineStyle = value;
}

S100_LineStyle S100_Hatch::GetLineStyle() 
{
	return lineStyle;
}

void S100_Hatch::SetLineStyleReference(S100_LineStyleReference* value) 
{
	lineStyleReference = *value;
}

S100_LineStyleReference* S100_Hatch::GetLineStyleReference()
{
	return &lineStyleReference;
}

void S100_Hatch::SetCompositeLineStyle(S100_CompositeLineStyle* value)
{
	compositeLineStyle = *value;
}

S100_CompositeLineStyle* S100_Hatch::GetCompositeLineStyle() 
{
	return &compositeLineStyle;
}

void S100_Hatch::SetDirection(S100_VectorPoint value) 
{
	direction = value;
}

S100_VectorPoint S100_Hatch::GetDirection() 
{
	return direction;
}

void S100_Hatch::SetDistance(std::wstring& value)
{
	distance = value;
}

std::wstring S100_Hatch::GetDistance() 
{
	return distance;
}