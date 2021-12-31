#include "stdafx.h"
#include "S100_SRGB.h"

S100_SRGB::S100_SRGB()
{

}

S100_SRGB::~S100_SRGB()
{

}

void S100_SRGB::GetContents(pugi::xml_node Node)
{
	for (pugi::xml_node instruction = Node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "red"))
		{
			if (!pugi::as_wide(instruction.child_value()).empty())
			{
				red = std::stoi(pugi::as_wide(instruction.child_value()));
			}
		}
		else if (!strcmp(instructionName, "green"))
		{
			if (!pugi::as_wide(instruction.child_value()).empty())
			{
				green = std::stoi(pugi::as_wide(instruction.child_value()));
			}
			
		}
		else if (!strcmp(instructionName, "blue"))
		{
			if (!pugi::as_wide(instruction.child_value()).empty())
			{
				blue = std::stoi(pugi::as_wide(instruction.child_value()));
			}
		}
	}
}

void S100_SRGB::SetRed(int value) 
{
	red = value;
}

int S100_SRGB::GetRed() 
{
	return red;
}

void S100_SRGB::SetGreen(int value) 
{
	green = value;
}

int S100_SRGB::GetGreen() 
{
	return green;
}

void S100_SRGB::SetBlue(int value) 
{
	blue = value;
}

int S100_SRGB::GetBlue() 
{
	return blue;
}
