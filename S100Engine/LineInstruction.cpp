#include "stdafx.h"
#include "LineInstruction.h"
#include "AbstractLineStyle.h"

LineInstruction::LineInstruction()
{
	pAbstractLineStyle = nullptr;
	hasDash = false;
}

LineInstruction::~LineInstruction()
{
	delete pAbstractLineStyle;
}