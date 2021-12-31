#pragma once
#include "EnumTypeOfDrawingInstruction.h"

class S100DrawingUnit
{
public:
	S100DrawingUnit();
	virtual ~S100DrawingUnit();

public:
	TypeOfDrawingInstruction type;
	int scaleMin;
	int scaleMax;
};