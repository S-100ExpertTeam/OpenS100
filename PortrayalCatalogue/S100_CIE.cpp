#include "stdafx.h"
#include "S100_CIE.h"

S100_CIE::S100_CIE()
{

}

S100_CIE::~S100_CIE()
{

}

#pragma warning(disable:4244)
void S100_CIE::GetContents(pugi::xml_node Node)
{
	if (Node == nullptr)
	{
		return;
	}

	for (pugi::xml_node instruction = Node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "xyL"))
		{
			for (pugi::xml_node child = instruction.first_child(); child; child = child.next_sibling())
			{
				auto childName = child.name();
				if (!strcmp(childName, "x"))
				{
					x = atof(child.child_value());
				}
				else if (!strcmp(childName, "y"))
				{
					y = atof(child.child_value());
				}
				else if (!strcmp(childName, "L"))
				{
					L = atof(child.child_value());
				}
			}
		}
	}
}

void S100_CIE::SetX(float value)
{
	x = value;
}

float S100_CIE::GetX() 
{
	return x;
}

void S100_CIE::SetY(float value) 
{
	y = value;
}

float S100_CIE::GetY() 
{
	return y;
}

void S100_CIE::SetL(float value) 
{
	L = value;
}

float S100_CIE::GetL() 
{
	return L;
}