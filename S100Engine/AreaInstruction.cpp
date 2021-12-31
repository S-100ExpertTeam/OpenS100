#include "stdafx.h"
#include "AreaInstruction.h"
#include "AbstractAreaFill.h"

AreaInstruction::AreaInstruction()
{
	pAbstractAreaFill = nullptr;
	colorName = _T("");
	transparency = 1;
	patternName = _T("");
}

AreaInstruction::~AreaInstruction()
{
	delete pAbstractAreaFill;
}