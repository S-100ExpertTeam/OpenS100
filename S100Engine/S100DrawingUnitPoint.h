#pragma once
#include "S100DrawingUnit.h"

class S100DrawingUnitPoint :
	public S100DrawingUnit
{
public:
	S100DrawingUnitPoint();
	virtual ~S100DrawingUnitPoint();

public:
	D2D1_POINT_2F point;
	CString symbolName;
	FLOAT rotation;
};