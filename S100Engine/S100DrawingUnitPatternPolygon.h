#pragma once
#include "S100DrawingUnit.h"

class S100DrawingUnitPatternPolygon :
	public S100DrawingUnit
{
public:
	S100DrawingUnitPatternPolygon();
	virtual ~S100DrawingUnitPatternPolygon();

public:
	CString patternName;
	ID2D1Geometry* pGeometry;
};