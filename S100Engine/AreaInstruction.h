#pragma once
#include "DrawingInstruction.h"

class AbstractAreaFill;
class AreaInstruction :
	public DrawingInstruction
{
public:
	AreaInstruction();
	virtual ~AreaInstruction();

public:
	AbstractAreaFill *pAbstractAreaFill;
	// Temporary addition.
	CString colorName;
	FLOAT transparency;
	CString patternName;
};