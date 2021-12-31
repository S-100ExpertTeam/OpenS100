#pragma once
#include "DrawingInstruction.h"

class CoverageFill;
class CoverageInstruction :
	public DrawingInstruction
{
public:
	CoverageInstruction();
	virtual ~CoverageInstruction();

public:
	CoverageFill *pCoverageFill;
};