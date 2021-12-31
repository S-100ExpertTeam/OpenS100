#include "stdafx.h"
#include "CoverageInstruction.h"
#include "CoverageFill.h"

CoverageInstruction::CoverageInstruction()
{
	pCoverageFill = nullptr;
}

CoverageInstruction::~CoverageInstruction()
{
	delete pCoverageFill;
}
