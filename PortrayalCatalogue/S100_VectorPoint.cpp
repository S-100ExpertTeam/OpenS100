#include "stdafx.h"
#include "S100_VectorPoint.h"

S100_VectorPoint::S100_VectorPoint()
{

}

S100_VectorPoint::~S100_VectorPoint()
{

}

void S100_VectorPoint::GetContents(pugi::xml_node& node)
{
	if (!node)
		return;
	SetX(node.child("x").text().as_double());
	SetX(node.child("y").text().as_double());

}

void S100_VectorPoint::SetX(std::wstring& value)
{
	x = std::stod(value);
}

void S100_VectorPoint::SetY(std::wstring& value)
{
	y = std::stod(value);
}
void S100_VectorPoint::SetX(double value)
{
	x = value;
}
void S100_VectorPoint::SetY(double value)
{
	y = value;
}
double S100_VectorPoint::GetX()
{
	return x;
}
double S100_VectorPoint::GetY()
{
	return y;
}