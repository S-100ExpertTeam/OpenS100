#pragma once
#include "SENC_Instruction.h"
#include "SENC_AreaFillBase.h"

class SENC_AreaInstruction : public SENC_Instruction
{
public:
	SENC_AreaInstruction();
	virtual ~SENC_AreaInstruction();

public:
	SENC_AreaFillBase *areaFill = nullptr;

public:
	void SetAreaFill(SENC_AreaFillBase* value);
	SENC_AreaFillBase* GetAreaFill();

	void DrawInstruction(
		D2D1Resources* d2,
		Scaler *scaler, 
		PortrayalCatalogue* pc = nullptr) override;

	void ChangePallete(PortrayalCatalogue *pc);

	void FromS100Instruction(
		S100_Instruction* s100Instruction, 
		PortrayalCatalogue *pc, 
		PCOutputSchemaManager* output);
};