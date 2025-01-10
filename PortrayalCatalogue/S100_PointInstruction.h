#pragma once
#include "S100_Instruction.h"
#include "S100_Symbol.h"
#include "S100_AugmentedPoint.h"

class S100_PointInstruction : public S100_Instruction
{
public:
	S100_PointInstruction();
	virtual ~S100_PointInstruction();

private:
	S100_Symbol *symbol = nullptr;
	S100_VectorPoint *vectorPoint = nullptr;

public:
	void GetContents(pugi::xml_node node);

	void SetSymbol(S100_Symbol* value);
	void SetSymbol(std::wstring& reference, double rotation = 0.0);
	void SetVectorPoint(S100_VectorPoint* value);
	void SetVectorPoint(std::wstring& x, std::wstring& y);

	S100_Symbol* GetSymbol();
	S100_VectorPoint* GetVectorPoint();
};