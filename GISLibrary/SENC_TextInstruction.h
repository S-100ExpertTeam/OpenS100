#pragma once
#include "SENC_Instruction.h"
#include "SENC_TextPoint.h"
#include "RenderCacheSlot.h"

class SENC_TextInstruction :
	public SENC_Instruction
{
public:
	SENC_TextInstruction();
	virtual ~SENC_TextInstruction();

public:
	SENC_TextPoint *textPoint;
	// Filled by the renderer (the measured text size); see RenderCacheSlot.h.
	GISLibrary::RenderCacheSlot renderCache;

public:
	void Accept(SENC_InstructionVisitor& visitor) override { visitor.Visit(*this); }
	void GetDrawPoints(Scaler *scaler, std::list<GISLibrary::PointF> &points);
	//void GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};