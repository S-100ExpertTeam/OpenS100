#include "stdafx.h"
#include "S100_Sector.h"

S100_Sector::S100_Sector()
{

}

S100_Sector::~S100_Sector()
{

}

void S100_Sector::GetContents(pugi::xml_node node)
{
	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "startAngle"))
		{
			startAngle = instruction.child_value();
		}
		else if (!strcmp(instructionName, "angularDistance"))
		{
			angularDistance = instruction.child_value();
		}
	}
}

void S100_Sector::SetStartAngle(const std::string& value)  { startAngle = value; }
void S100_Sector::SetStartAngle(const std::wstring& value) { startAngle = toUtf8(value); }
std::string  S100_Sector::GetStartAngle()  { return startAngle; }
std::wstring S100_Sector::GetStartAngleW() { return toWide(startAngle); }

void S100_Sector::SetAnglearDistance(const std::string& value)  { angularDistance = value; }
void S100_Sector::SetAnglearDistance(const std::wstring& value) { angularDistance = toUtf8(value); }
std::string  S100_Sector::GetAnglearDistance()  { return angularDistance; }
std::wstring S100_Sector::GetAnglearDistanceW() { return toWide(angularDistance); }
