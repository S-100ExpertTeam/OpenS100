#pragma once
#include "S100_Instruction.h"
#include "S100_TextPoint.h"

class S100_AugmentedGeometry : public S100_Instruction
{
public:
	S100_AugmentedGeometry();
	virtual ~S100_AugmentedGeometry();

private:
	std::wstring crsType;
	S100_VectorPoint* vectorPoint;
	S100_TextPoint* textPoint;

public:
	void SetCrsType(std::wstring& value);
	void SetVectorPoint(S100_VectorPoint* value);
	void SetTextPoint(S100_TextPoint* value);

	std::wstring GetCrsType();
	S100_VectorPoint* GetVectorPoint();
	S100_TextPoint* GetTextPoint();
};