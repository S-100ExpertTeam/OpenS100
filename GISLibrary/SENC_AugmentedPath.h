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
	void DrawInstruction(D2D1Resources* d2, Scaler *scaler, PortrayalCatalogue* pc = nullptr) override;
	void ChangePallete(PortrayalCatalogue *pc);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};