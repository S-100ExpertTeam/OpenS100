#include "stdafx.h"
#include "TextInstruction.h"
#include "Text.h"

TextInstruction::TextInstruction()
{
	m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::textInstruction;
}

TextInstruction::~TextInstruction()
{
	delete pText;
}
