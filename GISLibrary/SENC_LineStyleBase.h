#pragma once
#include <vector>

class PortrayalCatalogue;
class SCurve;
class Scaler;

// AbstractLineStyle
class SENC_LineStyleBase
{
public:
	SENC_LineStyleBase();
	virtual ~SENC_LineStyleBase();

public: 
	/*
	* 1 : Line Style
	* 2 : Line Style Reference
	* 3 : Composite Line Style
	*/
	int type = 0; 
	std::vector<ID2D1PathGeometry*> geometryVector;

public:
	void SetType(int value);
	int GetType();

	virtual void ChangePallete(PortrayalCatalogue *pc) {};

	virtual void DrawInstruction(
		SCurve* curveHasOrient,
		ID2D1DCRenderTarget* rt,
		ID2D1Factory1* pDirect2dFactory,
		ID2D1SolidColorBrush* brush,
		std::vector<ID2D1StrokeStyle1*>* strokeGroup,
		Scaler *scaler,
		PortrayalCatalogue* pc = nullptr) {};

	bool IsLineStyle();
	bool IsLineStyleReference();
	bool IsCompositeLineStyle();
};