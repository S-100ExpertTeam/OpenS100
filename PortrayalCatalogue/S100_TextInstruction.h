#pragma once
#include "S100_Instruction.h"
#include "S100_FeatureReference.h"
#include "S100_TextPoint.h"
#include "S100_VectorPoint.h"

class S100_TextInstruction : public S100_Instruction
{
public:
	S100_TextInstruction();
	virtual ~S100_TextInstruction();

private:
	S100_TextPoint* textPoint;

public:
	void GetContents(pugi::xml_node node);

	void SetTextPoint(S100_TextPoint* value);
	S100_TextPoint* GetTextPoint();
};