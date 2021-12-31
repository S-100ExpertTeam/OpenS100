#include "stdafx.h"
#include "TextInstruction.h"
#include "Text.h"

TextInstruction::TextInstruction()
{
	m_nTypeOfDrawingInstruction = textInstruction;
}

TextInstruction::~TextInstruction()
{
	delete pText;
}
