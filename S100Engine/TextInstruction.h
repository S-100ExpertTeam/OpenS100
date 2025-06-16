#pragma once

#include "DrawingInstruction.h"
#include "Text.h"

class Text;
class TextInstruction :
	public DrawingInstruction
{
public:
	TextInstruction();
	virtual ~TextInstruction();

public:
	TextPackage::Text *pText = nullptr;
};