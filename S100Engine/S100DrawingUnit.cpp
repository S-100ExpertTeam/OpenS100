#include "stdafx.h"
#include "S100DrawingUnit.h"

S100DrawingUnit::S100DrawingUnit()
{
	type = TypeOfDrawingInstruction::nullInstruction;
	scaleMin = INT32_MAX;
	scaleMax = 0;
}

S100DrawingUnit::~S100DrawingUnit()
{
	
}