#pragma once

#include "PivotPoint.h"

struct ID2D1PathGeometry;

struct SVGGeometry
{
	ID2D1PathGeometry* pGeometry;
	D2D1_ELLIPSE ellipse;
	bool bFill;
	bool bStoke;
	FLOAT width; // stroke width
	std::wstring strokeColorName;
	std::wstring fillColorName;
	FLOAT alpha;
};