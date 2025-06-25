 #include "stdafx.h"
#include "S100_ArcByRadius.h"

S100_ArcByRadius::S100_ArcByRadius()
{
	sector = nullptr;
}

S100_ArcByRadius::~S100_ArcByRadius()
{

}

void S100_ArcByRadius::GetContents(pugi::xml_node node) 
{
	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"center"))
		{
			center.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "sector"))
		{
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
	center = *value;
}

S100_VectorPoint* S100_ArcByRadius::GetCenter() 
{
	return &center;
}

void S100_ArcByRadius::SetSector(S100_Sector* value) 
{
	sector = value;
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
