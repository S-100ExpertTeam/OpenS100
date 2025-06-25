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
	int areaCRSType;
	ID2D1PathGeometry *pGeometry = nullptr;
	double direction;
	double length;

public:
	void DrawInstruction(D2D1Resources* d2, Scaler *scaler, PortrayalCatalogue* pc = nullptr) override;
	void ChangePallete(PortrayalCatalogue *pc);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};