#include "stdafx.h"
#include "S100_AugmentedGeometry.h"

S100_AugmentedGeometry::S100_AugmentedGeometry()
{
}

S100_AugmentedGeometry::~S100_AugmentedGeometry()
{
	if (vectorPoint)
	{
		delete vectorPoint;
		vectorPoint = nullptr;
	}

	if (textPoint)
	{
		delete textPoint;
		textPoint = nullptr;
	}
}

void S100_AugmentedGeometry::SetCrsType(const std::string& value)  { crsType = value; }
void S100_AugmentedGeometry::SetCrsType(const std::wstring& value) { crsType = toUtf8(value); }
std::string  S100_AugmentedGeometry::GetCrsType()  { return crsType; }
std::wstring S100_AugmentedGeometry::GetCrsTypeW() { return toWide(crsType); }

void S100_AugmentedGeometry::SetVectorPoint(S100_VectorPoint* value) { vectorPoint = value; }
void S100_AugmentedGeometry::SetTextPoint(S100_TextPoint* value)     { textPoint = value; }

S100_VectorPoint* S100_AugmentedGeometry::GetVectorPoint() { return vectorPoint; }
S100_TextPoint*   S100_AugmentedGeometry::GetTextPoint()   { return textPoint; }
