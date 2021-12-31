#pragma once
#include "SENC_WithParam.h"

#include <list>
#include <vector>

class SENC_Instruction;
class SENC_PointInstruction;
class SENC_LineInstruction;
class SENC_AreaInstruction;
class SENC_TextInstruction;
class SENC_CallTemplate
{
public:
	SENC_CallTemplate();
	virtual ~SENC_CallTemplate();

public:
	std::wstring name;
	std::vector<SENC_WithParam*> params;
	std::list<SENC_Instruction*> instructions;
	std::vector<SENC_PointInstruction*> pointInstructions;
	std::vector<SENC_LineInstruction*> curveInstructions;
	std::vector<SENC_AreaInstruction*> surfaceInstructions;
	std::vector<SENC_TextInstruction*> textInstructions;
};