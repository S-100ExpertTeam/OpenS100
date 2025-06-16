#pragma once
#include "SENC_Instruction.h"
#include "SENC_LineStyleBase.h"

#include <vector>
#include <unordered_map>

class SCurve;
class SENC_LineInstruction : public SENC_Instruction
{
public:
	SENC_LineInstruction();
	virtual ~SENC_LineInstruction();

private:
	bool suppression = false;

public:
	// multiplicity (0..*) to support composite line style.
	std::vector<SENC_LineStyleBase*> lineStyles;

public:
	void SetSuppression(bool value);
	bool GetSuppression();

	void DrawInstruction(
		D2D1Resources* d2,
		Scaler *scaler, 
		PortrayalCatalogue* pc = nullptr);
	
	void ChangePallete(PortrayalCatalogue *pc);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};