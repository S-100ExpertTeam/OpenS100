#pragma once
#include "DrawingInstruction.h"

class NullInstruction :
	public DrawingInstruction
{
public:
	NullInstruction();
	virtual ~NullInstruction();
};