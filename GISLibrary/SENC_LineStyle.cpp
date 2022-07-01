#include "stdafx.h"
#include "SENC_LineStyle.h"
#include "SENC_CommonFuc.h"
#include "GISLibrary.h"

#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\S100Geometry\\SCurveHasOrient.h"
#include "..\\S100Geometry\\SCurve.h"

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

void SENC_LineStyle::SetPenColor(D2D1_COLOR_F value)
{
	pen_color = value;
}

D2D1_COLOR_F SENC_LineStyle::GetPenColor()
{
	return pen_color;
}

void SENC_LineStyle::SetPenColorToken(std::wstring& value)
{
	colorToken = value;
}

std::wstring SENC_LineStyle::GetPenColorToken()
{
	return colorToken;
}

void SENC_LineStyle::GetStyleFromS100(S100_LineStyle* lineStyle, PortrayalCatalogue *pc)
{
	intervalLength = _wtof(lineStyle->GetIntervalLength().c_str());
	capStyle = SENC_CommonFuc::GetCapStyle(lineStyle->GetCapStyle());
	joinStyle = SENC_CommonFuc::GetJoinStyle(lineStyle->GetJoinStyle());
	offset = (float)(_wtof(lineStyle->GetOffset().c_str()));

	std::wstring colorToken = lineStyle->GetPen()->GetColor().GetToken();
	pen_width = _wtof(lineStyle->GetPen()->GetWidth().c_str()) + 0.01;
	pen_transparency = lineStyle->GetPen()->GetColor().GetTransparency();

	auto colorProfile = pc->GetS100PCManager()->GetS100ColorProfile();

	if (colorProfile)
	{
		SetPenColor(colorProfile->GetColor(colorToken));
		SetPenColorToken(colorToken);
	}

	auto dashs= lineStyle->GetDashs();
	for (auto itor = dashs.begin();
		itor != dashs.end();
		itor++)
	{
		S100_Dash dash = *itor;
		SENC_Dash* dashSENC = new SENC_Dash();
		dashSENC->length = (float)(_wtof(dash.GetLength().c_str()));
		dashSENC->start = (float)(_wtof(dash.GetStart().c_str()));

		dashes.push_back(dashSENC);
	}


	auto symbolslist = lineStyle->GetSymbols();
	for (auto itor = symbolslist.begin();
		itor != symbolslist.end();
		itor++)
	{
		S100_LineSymbol symbol = *itor;
		SENC_LineSymbol* symbolSENC = new SENC_LineSymbol();
		symbolSENC->reference = symbol.reference;
		symbolSENC->position = (float)(_wtof(symbol.position.c_str()));

		symbols.push_back(symbolSENC);
	}
}

void SENC_LineStyle::ChangePallete(PortrayalCatalogue *pc)
{
	SetPenColor(pc->GetS100PCManager()->GetS100ColorProfile()->GetColor(GetPenColorToken()));
}

void SENC_LineStyle::SetLineStyleName(std::wstring& value)
{
	lineStyleName = value;
}

std::wstring SENC_LineStyle::GetLineStyleName()
{
	return lineStyleName;
}

void SENC_LineStyle::DrawInstruction(
	SCurveHasOrient* curveHasOrient,
	ID2D1DCRenderTarget* rt,
	ID2D1Factory1* pDirect2dFactory,
	ID2D1SolidColorBrush* brush,
	std::vector<ID2D1StrokeStyle1*>* strokeGroup,
	Scaler *scaler,
	PortrayalCatalogue* pc)
{
	auto curve = curveHasOrient->GetCurve();
	auto name = GetLineStyleName();

	if (name != L"")
	{
		auto numPoints = curve->GetNumPoints();
		auto d2Points = new D2D1_POINT_2F[numPoints];

		bool curOrient = true;

		if (true == curveHasOrient->GetParentOrient())
		{
			curOrient = true;
		}
		else
		{
			curOrient = false;
		}

		if (curOrient)
		{
			for (int pi = 0; pi < numPoints; pi++)
			{
				scaler->WorldToDevice_F(
					curve->GetX(pi),
					curve->GetY(pi),
					&d2Points[pi].x,
					&d2Points[pi].y);
			}
		}
		else
		{
			for (int pi = 0; pi < numPoints; pi++)
			{
				scaler->WorldToDevice_F(
					curve->GetX(numPoints - 1 - pi),
					curve->GetY(numPoints - 1 - pi),
					&d2Points[pi].x,
					&d2Points[pi].y);
			}
		}

		auto s100PCManager = pc->GetS100PCManager();
		if (s100PCManager)
		{
			IWICBitmap* pIWICBitmap = s100PCManager->GetLineImage(GetLineStyleName() + _T(".xml"));

			if (!pIWICBitmap)
			{
				std::wstring nameQUESMRK1(L"QUESMRK1.xml");
				pIWICBitmap = s100PCManager->GetLineImage(nameQUESMRK1);

				if (!pIWICBitmap)
				{
					return;
				}
			}

			if (pIWICBitmap)
			{
				ID2D1Bitmap* pBitmap = nullptr;
				HRESULT hr = rt->CreateBitmapFromWicBitmap(pIWICBitmap, &pBitmap);

				if (!SUCCEEDED(hr))
				{
					OutputDebugString(_T("Failed to create bitmap\n"));
				}
				else
				{
					rt->SetTransform(D2D1::Matrix3x2F::Identity());
					// Point rotate
					if (scaler->GetRotationDegree())
					{
						double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

						for (auto id = 0; id < numPoints; id++)
						{
							auto p = &d2Points[id];
							FLOAT tempX = (float)(scaler->soy - p->y) * (float)sin(radian) + (float)(scaler->sox - p->x) * (float)cos(radian);
							FLOAT tempY = (float)(scaler->soy - p->y) * (float)cos(radian) - (float)(scaler->sox - p->x) * (float)sin(radian);

							p->x = (float)tempX + (float)scaler->sox;
							p->y = (float)tempY + (float)scaler->soy;
						}
					}

					gisLib->s100Render.DrawBitmapOnPolyline(rt, d2Points, numPoints, pBitmap, scaler->GetD2Rect(), GetPenColor(), strokeGroup->at(1));
					SafeRelease(&pBitmap);
				}
			}
		}

		delete[] d2Points;
		d2Points = nullptr;
	}
	else
	{
		auto PEN_COLOR = GetPenColor();
		auto PEN_WIDTH = (float)((pen_width + 0.01) / 0.32);

		D2D1_COLOR_F color = PEN_COLOR;
		color.a = 1;
		brush->SetColor(color);

		if (dashes.size() > 0)
		{
			float dashes[] = { (float)(this->dashes.front()->length + 0.01) / (float)(0.32 * PEN_WIDTH), (float)(intervalLength - this->dashes.front()->length + 0.01) / (float)(0.32 * PEN_WIDTH) };
			ID2D1StrokeStyle1* dash = nullptr;
			pDirect2dFactory->CreateStrokeStyle(
				D2D1::StrokeStyleProperties1(
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_ROUND,
					D2D1_LINE_JOIN_MITER,
					10.0f,
					D2D1_DASH_STYLE_CUSTOM,
					0.0f,
					D2D1_STROKE_TRANSFORM_TYPE_FIXED
				),
				dashes,
				ARRAYSIZE(dashes),
				&dash
			);

			if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
			{
				auto geometry = curve->GetD2Geometry();
				if (geometry)
				{
					rt->DrawGeometry(geometry, brush, PEN_WIDTH, dash);
				}
			}
			else
			{
				auto geometry = curve->GetNewD2Geometry(pDirect2dFactory, scaler);
				if (geometry)
				{
					D2D1_MATRIX_3X2_F oldMatrix;
					rt->GetTransform(&oldMatrix);
					rt->SetTransform(D2D1::Matrix3x2F::Identity());
					rt->SetTransform(
						D2D1::Matrix3x2F::Rotation(
							(float)scaler->GetRotationDegree(),
							D2D1::Point2F((float)scaler->sox, (float)scaler->soy))
					);
					rt->DrawGeometry(geometry, brush, PEN_WIDTH, dash);
					rt->SetTransform(oldMatrix);
					SafeRelease(&geometry);
				}
			}

			SafeRelease(&dash);
		}
		else if (symbols.size() > 0)
		{

		}
		else
		{
			if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
			{
				auto geometry = curve->GetD2Geometry();
				if (geometry)
				{
					rt->DrawGeometry(geometry, brush, PEN_WIDTH, (*strokeGroup)[0]);
				}
			}
			else
			{
				//Memory leak
				auto geometry = curve->GetNewD2Geometry(pDirect2dFactory, scaler);
				if (geometry)
				{
					D2D1_MATRIX_3X2_F oldMatrix;
					rt->GetTransform(&oldMatrix);
					rt->SetTransform(D2D1::Matrix3x2F::Identity());
					rt->SetTransform(
						D2D1::Matrix3x2F::Rotation(
							(float)scaler->GetRotationDegree(),
							D2D1::Point2F((float)scaler->sox, (float)scaler->soy))
					);
					rt->DrawGeometry(geometry, brush, PEN_WIDTH, (*strokeGroup)[0]);
					rt->SetTransform(oldMatrix);
					SafeRelease(&geometry);
				}
			}
		}
	}
}