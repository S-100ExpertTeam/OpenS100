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
			startAngle = pugi::as_wide(instruction.value());
		}
		else if (!strcmp(instructionName, "angularDistance"))
		{
			angularDistance = pugi::as_wide(instruction.value());
		}
	}
}

void S100_Sector::SetStartAngle(std::wstring& value)
{
	startAngle = value;
}

void S100_Sector::SetStartAngle(std::string& value)
{
	if (value.empty())
		startAngle = _T("");
	else
	{
		std::wstring StartAngle(value.begin(), value.end());
		SetStartAngle(StartAngle);
	}
}

void S100_Sector::SetAnglearDistance(std::wstring& value)
{
	angularDistance = value;
}

void S100_Sector::SetAnglearDistance(std::string& value)
{
	if (value.empty())
		angularDistance = _T("");
	else
	{
		std::wstring AnglearDistance(value.begin(), value.end());
		SetAnglearDistance(AnglearDistance);
	}
}

std::wstring S100_Sector::GetStartAngle()
{
	return startAngle;
}
 
std::wstring S100_Sector::GetAnglearDistance()
{
	return angularDistance;
}