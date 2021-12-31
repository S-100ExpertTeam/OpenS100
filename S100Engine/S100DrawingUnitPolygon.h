#pragma once
#include "S100DrawingUnit.h"

#include <vector>


class S100DrawingUnitPolygon : public S100DrawingUnit
{
public:
	S100DrawingUnitPolygon();
	virtual ~S100DrawingUnitPolygon();

public:
	D2D1_COLOR_F color;
	FLOAT transparency;
	ID2D1Geometry* pGeometry;
	D2D1_RECT_F bounds;
	std::vector<D2D1_POINT_2F> points; // only have the edges.
};