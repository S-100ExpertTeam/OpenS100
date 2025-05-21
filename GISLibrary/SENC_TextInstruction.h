#pragma once
#include "SENC_Instruction.h"
#include "SENC_TextPoint.h"

class SENC_TextInstruction :
	public SENC_Instruction
{
public:
	SENC_TextInstruction();
	virtual ~SENC_TextInstruction();

public:
	SENC_TextPoint *textPoint;
	DWRITE_TEXT_METRICS* textMatrix = nullptr;

public:
	//void GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points);
	//void GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points);
	void ChangePallete(PortrayalCatalogue* pc);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};