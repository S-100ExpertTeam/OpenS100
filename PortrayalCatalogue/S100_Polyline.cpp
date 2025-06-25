#include "stdafx.h"
#include "S100_Polyline.h"

S100_Polyline::S100_Polyline()
{

}

S100_Polyline::~S100_Polyline()
{

}

void S100_Polyline::GetContents(pugi::xml_node node)
{
	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,""))
		{
			S100_VectorPoint point;
			point.GetContents(instruction);
			points.push_back(point);
		}
		else
		{
			continue;
		}
	}
}

void S100_Polyline::SetPoints(S100_VectorPoint* value) 
{
	points.push_back(*value);
}

void S100_Polyline::SetPoints(std::list<S100_VectorPoint> value)
{
	points = value;
}

S100_VectorPoint* S100_Polyline::GetPoints(int index) 
{
	auto it = points.begin();
	std::advance(it, index);

	return &(*it);
}

std::list<S100_VectorPoint> S100_Polyline::GetPoints() 
{
	return points;
}