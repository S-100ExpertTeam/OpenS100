#include "stdafx.h"
#include "S100_Path.h"

S100_Path::S100_Path()
{
}

S100_Path::~S100_Path()
{
	for (auto value : arcByRadiuses)
	{
		value.DeleteLeftPtr();
	}
}

void S100_Path::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}


	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();

		if (strcmp(instructionName, "polyline"))
		{
			S100_Polyline polyline;
			polyline.GetContents(instruction);
			polylines.push_back(polyline);
		}
		else if (strcmp(instructionName, "arc3Points"))
		{
			S100_Arc3Points arc3Points;
			arc3Points.GetContents(instruction);
			arc3Pointses.push_back(arc3Points);
		}
		else if (strcmp(instructionName, "arcByRadius"))
		{
			S100_ArcByRadius arcByRadius;
			arcByRadius.GetContents(instruction);
			arcByRadiuses.push_back(arcByRadius);

		}
		else if (strcmp(instructionName, "annulus"))
		{
			S100_Annulus annulus;
			annulus.GetContents(instruction);
			annuluses.push_back(annulus);
		}
	}
}

void S100_Path::SetPolylines(S100_Polyline* value) 
{
	polylines.push_back(*value);
}

void S100_Path::SetPolylines(std::vector<S100_Polyline> value)
{
	polylines = value;
}

S100_Polyline* S100_Path::GetPolylines(int index) 
{
	return &polylines.at(index);
}

std::vector<S100_Polyline> S100_Path::GetPolylines() 
{
	return polylines;
}

void S100_Path::SetArc3Pointses(S100_Arc3Points* value) 
{
	arc3Pointses.push_back(*value);
}

void S100_Path::SetArc3Pointses(std::vector<S100_Arc3Points> value) 
{
	arc3Pointses = value;
}

S100_Arc3Points* S100_Path::GetArc3Pointses(int index)
{
	return &arc3Pointses.at(index);
}

std::vector<S100_Arc3Points> S100_Path::GetArc3Pointses()
{
	return arc3Pointses;
}

void S100_Path::SetArcByRadiuses(S100_ArcByRadius* value)
{
	arcByRadiuses.push_back(*value);
}

void S100_Path::SetArcByRadiuses(std::vector<S100_ArcByRadius> value) 
{
	arcByRadiuses = value;
}

S100_ArcByRadius* S100_Path::GetArcByRadiuses(int index) 
{
	return &arcByRadiuses.at(index);
}

std::vector<S100_ArcByRadius> S100_Path::GetArcByRadiuses()
{
	return arcByRadiuses;
}

void S100_Path::SetAnnuluses(S100_Annulus* value) 
{
	annuluses.push_back(*value);
}

void S100_Path::SetAnnuluses(std::vector<S100_Annulus> value) 
{
	annuluses = value;
}

S100_Annulus* S100_Path::GetAnnuluses(int index) 
{
	return &annuluses.at(index);
}

std::vector<S100_Annulus> S100_Path::GetAnnuluses() 
{
	return annuluses;
}