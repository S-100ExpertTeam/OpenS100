#include "stdafx.h"
#include "SENC_LineStyle.h"
#include "SENC_CommonFuc.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "SCommonFuction.h"
#include "SCurve.h"

#include "../LatLonUtility/LatLonUtility.h"

SENC_LineStyle::SENC_LineStyle() : SENC_LineStyleBase()
{
	type = 1;
}

SENC_LineStyle::~SENC_LineStyle()
{
	for (auto itor = symbols.begin(); itor != symbols.end(); itor++)
	{
		delete *itor;
	}
	for (auto itor = dashes.begin(); itor != dashes.end(); itor++)
	{
		delete *itor;
	}
}

void SENC_LineStyle::SetPenColorToken(std::wstring& value)
{
	colorToken = value;
}

std::wstring SENC_LineStyle::GetPenColorToken()
{
	return colorToken;
}

void SENC_LineStyle::GetStyleFromS100(S100_LineStyle* lineStyle, PortrayalCatalogue* /*pc*/)
{
	intervalLength = std::wcstod(lineStyle->GetIntervalLengthW().c_str(), nullptr);
	capStyle = SENC_CommonFuc::GetCapStyle(lineStyle->GetCapStyleW());
	joinStyle = SENC_CommonFuc::GetJoinStyle(lineStyle->GetJoinStyleW());
	offset = (float)(std::wcstod(lineStyle->GetOffsetW().c_str(), nullptr));

	std::wstring _colorToken = lineStyle->GetPen()->GetColor().GetTokenW();
	pen_width = std::wcstod(lineStyle->GetPen()->GetWidthW().c_str(), nullptr) + 0.01;
	pen_transparency = lineStyle->GetPen()->GetColor().GetTransparency();
	SetPenColorToken(_colorToken);

	auto dashs= lineStyle->GetDashs();
	for (auto itor = dashs.begin();
		itor != dashs.end();
		itor++)
	{
		S100_Dash dash = *itor;
		SENC_Dash* dashSENC = new SENC_Dash();
		dashSENC->length = (float)(std::wcstod(dash.GetLengthW().c_str(), nullptr));
		dashSENC->start = (float)(std::wcstod(dash.GetStartW().c_str(), nullptr));

		dashes.push_back(dashSENC);
	}


	auto symbolslist = lineStyle->GetSymbols();
	for (auto itor = symbolslist.begin();
		itor != symbolslist.end();
		itor++)
	{
		S100_LineSymbol symbol = *itor;
		SENC_LineSymbol* symbolSENC = new SENC_LineSymbol();
		symbolSENC->reference = symbol.GetReferenceW();
		symbolSENC->position = (float)(std::wcstod(symbol.GetPositionW().c_str(), nullptr));

		symbols.push_back(symbolSENC);
	}
}

void SENC_LineStyle::SetLineStyleName(const std::wstring& value)
{
	lineStyleName = value;
}

std::wstring SENC_LineStyle::GetLineStyleName()
{
	return lineStyleName;
}
