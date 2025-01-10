 #include "stdafx.h"
#include "S100_ArcByRadius.h"

S100_ArcByRadius::S100_ArcByRadius()
{
	center = nullptr;
	sector = nullptr;
}

S100_ArcByRadius::~S100_ArcByRadius()
{
	if (center)
		delete center, center = nullptr;

	if (sector)
		delete sector, sector = nullptr;
}

void S100_ArcByRadius::GetContents(pugi::xml_node node) 
{
	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"center"))
		{
			if (!center)
				center = new S100_VectorPoint();
			center->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "sector"))
		{
			if (!sector)
				sector = new S100_Sector();
			sector->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "radius"))
		{
			radius = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_ArcByRadius::SetCenter(S100_VectorPoint* value)
{
	center = value;
}

void S100_ArcByRadius::SetCenter(std::wstring& x, std::wstring& y)
{
	if (!center)
		center = new S100_VectorPoint();

	center->SetX(x);
	center->SetY(y);
}

S100_VectorPoint* S100_ArcByRadius::GetCenter() 
{
	return center;
}

void S100_ArcByRadius::SetSector(S100_Sector* value) 
{
	sector = value;
}

void S100_ArcByRadius::SetSector(std::wstring& angle, std::wstring& distance)
{
	if (!sector)
		sector = new S100_Sector();

	sector->SetStartAngle(angle);
	sector->SetAnglearDistance(distance);
}

S100_Sector* S100_ArcByRadius::GetSector() 
{
	return sector;
}

void S100_ArcByRadius::SetRadius(std::wstring& value)
{
	radius = value;
}

std::wstring S100_ArcByRadius::GetRadius() 
{
	return radius;
}

void S100_ArcByRadius::DeleteLeftPtr()
{
	if (sector != nullptr)
	{
		delete sector;
		sector = nullptr;
	}
}
