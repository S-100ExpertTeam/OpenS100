#include "stdafx.h"
#include "S100_Arc3Points.h"

S100_Arc3Points::S100_Arc3Points()
{
	point = nullptr;
	medianPoint = nullptr;
	endPoint = nullptr;
}

S100_Arc3Points::~S100_Arc3Points()
{
	if (point)
		delete point, point = nullptr;

	if (medianPoint)
		delete medianPoint, medianPoint = nullptr;

	if (endPoint)
		delete endPoint, endPoint = nullptr;
}

void S100_Arc3Points::GetContents(pugi::xml_node node)
{
	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "point"))
		{
			if (!point)
				point = new S100_VectorPoint();
			point->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "medianPoint"))
		{
			if (!medianPoint)
				medianPoint = new S100_VectorPoint();
			medianPoint->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "endPoint"))
		{
			if (!endPoint)
				endPoint = new S100_VectorPoint();
			endPoint->GetContents(instruction);
		}
	}
}

void S100_Arc3Points::SetPoint(S100_VectorPoint* value)
{
	point = value;
}

S100_VectorPoint* S100_Arc3Points::GetPoint()
{
	return point;
}

void S100_Arc3Points::SetMedianPoint(S100_VectorPoint* value) 
{
	medianPoint = value;
}

S100_VectorPoint* S100_Arc3Points::GetMedianPoint()
{
	return medianPoint;
}

void S100_Arc3Points::SetEndPoint(S100_VectorPoint* value)
{
	endPoint = value;
}

S100_VectorPoint* S100_Arc3Points::GetEndPoint()
{
	return endPoint;
}