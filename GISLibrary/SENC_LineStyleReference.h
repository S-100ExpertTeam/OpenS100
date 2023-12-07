#pragma once
#include "SENC_LineStyleBase.h"
#include "string"

class S100_LineStyleReference;
class SENC_LineStyleReference : public SENC_LineStyleBase
{
public:
	SENC_LineStyleReference();
	virtual ~SENC_LineStyleReference();

public:
	std::wstring reference;
	S100Render s100Render;

public:
	void GetStyleFromS100(S100_LineStyleReference* lineStyle);

	void SetReference(std::wstring& value);
	std::wstring GetReference();

public: // ovveride from SENC_LineStyleBase
	void DrawInstruction(
		SCurve* curveHasOrient,
		ID2D1DCRenderTarget* rt,
		ID2D1Factory1* pDirect2dFactory,
		ID2D1SolidColorBrush* brush,
		std::vector<ID2D1StrokeStyle1*>* strokeGroup,
		Scaler *scaler,
		PortrayalCatalogue* pc = nullptr) override;
};