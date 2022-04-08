#pragma once

#include "SENC_Instruction.h"
#include "SENC_Symbol.h"

class SENC_PointInstruction :
	public SENC_Instruction
{
public:
	SENC_PointInstruction();
	virtual ~SENC_PointInstruction();

public:
	SENC_Symbol *symbol = nullptr;
	SENC_VectorPoint *vectorPoint = nullptr;

public:
	void GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points);
	void GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};