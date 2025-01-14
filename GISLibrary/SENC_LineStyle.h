#pragma once
#include "SENC_LineStyleBase.h"
#include "SENC_Symbol.h"
#include "SENC_Dash.h"
#include "SENC_LineSymbol.h"

#include "..\\PortrayalCatalogue\\S100_LineStyle.h"

#include <list>

class PortrayalCatalogue;
class Scaler;
class SENC_LineStyle : public SENC_LineStyleBase
{
public:
	SENC_LineStyle();
	virtual ~SENC_LineStyle();
	
public:
	double intervalLength = 0;	
	double pen_width = 0;
	double pen_transparency = 0;

	std::list<SENC_Dash*> dashes;
	std::list<SENC_LineSymbol*> symbols;

	int capStyle = 0;
	int joinStyle = 0;
	float offset = 0;
	D2D1_COLOR_F pen_color;
	std::wstring colorToken = L"";
	std::wstring lineStyleName = L"";

	S100Render s100Render;
public:
	void SetPenColor(D2D1_COLOR_F value);
	D2D1_COLOR_F GetPenColor();

	void SetPenColorToken(std::wstring& value);
	std::wstring GetPenColorToken();

	void GetStyleFromS100(S100_LineStyle* lineStyle, PortrayalCatalogue *pc);
	void GetStyleFromString(std::string& lineStyle, std::string& dash, PortrayalCatalogue* pc);
	
	void ChangePallete(PortrayalCatalogue *pc);

	void SetLineStyleName(std::wstring& value);
	std::wstring GetLineStyleName();

public: // override from SENC_LineStyleBase
	void DrawInstruction(
		SCurve* curveHasOrient,
		ID2D1DCRenderTarget* rt,
		ID2D1Factory1* pDirect2dFactory,
		ID2D1SolidColorBrush* brush,
		std::vector<ID2D1StrokeStyle1*>* strokeGroup,
		Scaler *scaler,
		PortrayalCatalogue* pc = nullptr) override;
};