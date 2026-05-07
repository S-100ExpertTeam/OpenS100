#include "stdafx.h"
#include "S100_SRGB.h"

#include "..\\LatLonUtility\\cpp_util.h"

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
			const char* val = instruction.child_value();
			if (val && *val) red = cpp_util::stoi(val);
		}
		else if (!strcmp(instructionName, "green"))
		{
			const char* val = instruction.child_value();
			if (val && *val) green = cpp_util::stoi(val);
		}
		else if (!strcmp(instructionName, "blue"))
		{
			const char* val = instruction.child_value();
			if (val && *val) blue = cpp_util::stoi(val);
		}
	}
}

void S100_SRGB::SetRed(int value)   { red = value; }
int  S100_SRGB::GetRed()            { return red; }
void S100_SRGB::SetGreen(int value) { green = value; }
int  S100_SRGB::GetGreen()          { return green; }
void S100_SRGB::SetBlue(int value)  { blue = value; }
int  S100_SRGB::GetBlue()           { return blue; }
