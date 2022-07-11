#pragma once
#include "SENC_Instruction.h"
#include "SENC_LineStyleBase.h"

#include <vector>
#include <unordered_map>

class SCurveHasOrient;
class SENC_LineInstruction : public SENC_Instruction
{
public:
	SENC_LineInstruction();
	virtual ~SENC_LineInstruction();

private:
	bool suppression = false;

public:
	std::vector<SENC_LineStyleBase*> lineStyles;
	// Referenced Curve Link
	//std::list<SCurveHasOrient*> m_listCurveLink;

public:
	void SetSuppression(bool value);
	bool GetSuppression();

	void DrawInstruction(
		ID2D1DCRenderTarget* rt, 
		ID2D1Factory1* pDirect2dFactory, 
		ID2D1SolidColorBrush* brush, 
		std::vector<ID2D1StrokeStyle1*>* strokeGroup, 
		Scaler *scaler, 
		PortrayalCatalogue* pc = nullptr);
	
	void ChangePallete(PortrayalCatalogue *pc);
	void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output);
};