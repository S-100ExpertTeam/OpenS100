#pragma once
#include "S100_Instruction.h"
#include "S100_Symbol.h"
#include "S100_AugmentedPoint.h"

class S100_PointInstruction : public S100_Instruction
{
public:
	S100_PointInstruction();
	virtual ~S100_PointInstruction();

private:
	S100_Symbol *symbol = nullptr;

public:
	void GetContents(pugi::xml_node node);

	void SetSymbol(S100_Symbol* value);

	S100_Symbol* GetSymbol();
};