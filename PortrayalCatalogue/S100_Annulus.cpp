#include "stdafx.h"
#include "S100_Annulus.h"

S100_Annulus::S100_Annulus()
{

}

S100_Annulus::~S100_Annulus()
{
	delete innerRadius;
}

void S100_Annulus::GetContents(pugi::xml_node node)
{
	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "point"))
		{
			point.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "innerRadius"))
		{
			innerRadius = new std::string(instruction.child_value());
		}
		else if (!strcmp(instructionName, "outerRadius"))
		{
			outerRadius = instruction.child_value();
		}
		else if (!strcmp(instructionName, "sector"))
		{
			sector = new S100_Sector();
			sector->GetContents(instruction);
		}
	}
}

void S100_Annulus::SetPoint(S100_VectorPoint* value) { point = *value; }
S100_VectorPoint* S100_Annulus::GetPoint()           { return &point; }

void S100_Annulus::SetInnerRadius(std::string* value) { innerRadius = value; }
std::string* S100_Annulus::GetinnerRadius()           { return innerRadius; }

void S100_Annulus::SetOuterRadius(const std::string& value)  { outerRadius = value; }
void S100_Annulus::SetOuterRadius(const std::wstring& value) { outerRadius = toUtf8(value); }
std::string  S100_Annulus::GetOuterRadius()  { return outerRadius; }
std::wstring S100_Annulus::GetOuterRadiusW() { return toWide(outerRadius); }

void S100_Annulus::SetSector(S100_Sector* value) { sector = value; }
S100_Sector* S100_Annulus::GetSector()           { return sector; }
