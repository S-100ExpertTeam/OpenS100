#pragma once
#include "SENC_Instruction.h"
#include "SENC_LineStyleBase.h"
#include "SENC_Path.h"

class SENC_AugmentedPath :
	public SENC_Instruction
{
public:
	SENC_AugmentedPath();
	virtual ~SENC_AugmentedPath();

public:
	int areaCRSType;
	SENC_Path *path;
	std::vector<SENC_LineStyleBase*> lineStyles;

public:
	void Accept(SENC_InstructionVisitor& visitor) override { visitor.Visit(*this); }
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};