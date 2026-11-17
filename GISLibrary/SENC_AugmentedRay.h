#pragma once
#include "SENC_Instruction.h"
#include "SENC_LineStyleBase.h"

class SENC_AugmentedRay :
	public SENC_Instruction
{
public:
	SENC_AugmentedRay();
	virtual ~SENC_AugmentedRay();

public:
	std::vector<SENC_LineStyleBase*> lineStyles;
	int areaCRSType = 0;
	double direction = 0;
	double length = 0;

public:
	void Accept(SENC_InstructionVisitor& visitor) override { visitor.Visit(*this); }
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};