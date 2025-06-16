#pragma once
#include "DrawingInstruction.h"

class Symbol;
class PointInstruction :
	public DrawingInstruction
{
public:
	PointInstruction();
	virtual ~PointInstruction();

public:
	Symbol* m_pSymbol;
	FLOAT rotation;
	CString symbolName;
};