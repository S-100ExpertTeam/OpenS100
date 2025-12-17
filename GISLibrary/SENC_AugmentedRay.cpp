#include "stdafx.h"
#include "SENC_AugmentedRay.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"
#include "D2D1Resources.h"

#include "../GISLibrary/R_FeatureRecord.h"

#include "SCommonFuction.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../PortrayalCatalogue/S100_AugmentedRay.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../LatLonUtility/LatLonUtility.h"

#include <ctime> 
#include <mmsystem.h> 

const float factor = 3.5;

SENC_AugmentedRay::SENC_AugmentedRay()
{
	type = 7;
}

SENC_AugmentedRay::~SENC_AugmentedRay()
{
	for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
	{
		switch ((*itor)->type)
		{
		case 0:
			delete *itor;
			break;
		case 1:
			delete ((SENC_LineStyle*)*itor);
			break;
		case 2:
			delete ((SENC_LineStyleReference*)*itor);
			break;
		case 3:
			delete ((SENC_CompositeLineStyle*)*itor);
			break;
		}
	}

	if (pGeometry)
		SafeRelease(& pGeometry);
}

DWORD dwTimeCheck_AumentedRayInstruction[100] = { 0 };
int iDebugCount_AumentedRayInstruction[100] = { 0 };

#pragma warning(disable:4244)
#pragma warning(disable:4838)
void SENC_AugmentedRay::DrawInstruction(D2D1Resources* d2, Scaler *scaler, PortrayalCatalogue* pc)
{
	auto factory = d2->Factory();
	auto brush = d2->SolidColorBrush();
	auto rt = d2->RenderTarget();
	auto solidStroke = d2->SolidStrokeStyle();

	if (!factory || !brush || !rt || !solidStroke)
	{
		return;
	}

	if (nullptr == fr || nullptr == fr->GetGeometry())
	{
		return;
	}

	auto geom = fr->GetGeometry();

	if (geom->GetType() == SGeometryType::Point)
	{
		auto p = (SPoint*)geom;

		scaler->WorldToDevice(
			p->x, 
			p->y,
			&SGeometry::viewPoints[0].x, 
			&SGeometry::viewPoints[0].y);

		for (auto i = lineStyles.begin(); i != lineStyles.end(); i++)
		{
			auto lineStyle = *i;

			if (lineStyle->IsLineStyle() == true)
			{
				SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;
				//int PEN_COLOR = ls->GetPenColor();
				int PEN_WIDTH = (int)(ls->pen_width / 0.319);

				D2D1_POINT_2F pBeginningPt = { (FLOAT)SGeometry::viewPoints[0].x, (FLOAT)SGeometry::viewPoints[0].y };
				D2D1_POINT_2F pEndPt;

				// Point rotate
				if (scaler->GetRotationDegree())
				{
					double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

					FLOAT tempX = (scaler->soy - pBeginningPt.y) * sin(radian) + (scaler->sox - pBeginningPt.x) * cos(radian);
					FLOAT tempY = (scaler->soy - pBeginningPt.y) * cos(radian) - (scaler->sox - pBeginningPt.x) * sin(radian);

					pBeginningPt.x = tempX + scaler->sox;
					pBeginningPt.y = tempY + scaler->soy;
				}

				long ix = 0;
				long iy = 0; 
				double mx = p->x;
				double my = p->y;

				if (false == ENCCommon::FULL_SECTORS)
				{
					ix = pBeginningPt.x + (int)((cos((((int)direction - 90) % 360) * M_PI / 180) * length) * factor);
					iy = pBeginningPt.y + (int)((sin((((int)direction - 90) % 360) * M_PI / 180) * length) * factor);

					// Point rotate
					if (scaler->GetRotationDegree())
					{
						double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

						FLOAT tempX = (pBeginningPt.y - iy) * sin(radian) + (pBeginningPt.x - ix) * cos(radian);
						FLOAT tempY = (pBeginningPt.y - iy) * cos(radian) - (pBeginningPt.x - ix) * sin(radian);

						ix = tempX + pBeginningPt.x;
						iy = tempY + pBeginningPt.y;
					}
				}
				else
				{
					inverseProjection(mx, my);
					auto dest = LatLonUtility::GetDestinationPointRhumbLineMercatorByGeographicLib(my, mx, direction, length);
					projection(dest.lon, dest.lat);
					scaler->WorldToDevice(dest.lon, dest.lat, &ix, &iy);

					// Point rotate
					if (scaler->GetRotationDegree())
					{
						double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

						FLOAT tempX = (scaler->soy - iy) * sin(radian) + (scaler->sox - ix) * cos(radian);
						FLOAT tempY = (scaler->soy - iy) * cos(radian) - (scaler->sox - ix) * sin(radian);

						ix = tempX + scaler->sox;
						iy = tempY + scaler->soy;
					}
				}

				HRESULT hr = factory->CreatePathGeometry(&pGeometry);

				pEndPt.x = (FLOAT)ix;
				pEndPt.y = (FLOAT)iy;


				if (SUCCEEDED(hr))
				{
					ID2D1GeometrySink *pSink = nullptr;

					hr = pGeometry->Open(&pSink);

					if (SUCCEEDED(hr))
					{
						pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
						pSink->BeginFigure(
							pBeginningPt,
							D2D1_FIGURE_BEGIN_FILLED
						);

						pSink->AddLine(pEndPt);

						pSink->EndFigure(D2D1_FIGURE_END_OPEN);

						hr = pSink->Close();

						if (SUCCEEDED(hr))
						{
								
						}
						else
						{
							pGeometry = NULL;
						}
					}
				}
				
				brush->SetColor(ls->pen_color);

				if (ls->dashes.size() > 0 || ls->symbols.size() > 0)
				{
					float dashes[] = { (ls->dashes.front()->length + 0.01) / (0.32 * PEN_WIDTH), (ls->intervalLength - ls->dashes.front()->length + 0.01) / (0.32 * PEN_WIDTH) };
					ID2D1StrokeStyle1* dash = nullptr;
					factory->CreateStrokeStyle(
						D2D1::StrokeStyleProperties1(
							D2D1_CAP_STYLE_FLAT,
							D2D1_CAP_STYLE_FLAT,
							D2D1_CAP_STYLE_ROUND,
							D2D1_LINE_JOIN_MITER,
							10.0f,
							D2D1_DASH_STYLE_CUSTOM,
							0.0f
						),
						dashes,
						ARRAYSIZE(dashes),
						&dash
					);
					rt->DrawGeometry(pGeometry, brush, PEN_WIDTH, dash);
					SafeRelease(&dash);
				}
				else
				{
					rt->DrawGeometry(pGeometry, brush, PEN_WIDTH, solidStroke);
				}

				SafeRelease(&pGeometry);
			}
			else if (lineStyle->IsLineStyleReference() == true)
			{
			}
			else if (lineStyle->IsCompositeLineStyle() == true)
			{
			}
		}
	}
}

void SENC_AugmentedRay::ChangePallete(PortrayalCatalogue *pc)
{
	for (auto i = lineStyles.begin(); i != lineStyles.end(); i++)
	{
		(*i)->ChangePallete(pc);
	}
}

void SENC_AugmentedRay::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100AugmentedRay = (S100_AugmentedRay*)s100Instruction;

	output->GetSENCFromS100Common(s100Instruction, this);

	direction = _wtof(s100AugmentedRay->GetDirection().c_str());
	length = _wtof(s100AugmentedRay->GetLength().c_str());

	areaCRSType = SENC_CommonFuc::GetAreaCRSType(s100AugmentedRay->GetCrsType());

	if (s100AugmentedRay->GetCompositeLineStyle())
	{
		auto linestyleslist = s100AugmentedRay->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = linestyleslist.begin();
			itor != linestyleslist.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100((S100_LineStyle*)*itor, pc);

			//for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
			//{
			//	SENC_LineSymbol* symbol = *itorSymbol;
			//	auto svgsymbolManager = pc->GetSVGManager();
			//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
			//	if (itor != svgsymbolManager->m_svgMap.end())
			//	{
			//		symbol->pSvg = itor->second;
			//	}
			//	else
			//	{
			//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
			//		if (itor != svgsymbolManager->m_svgMap.end())
			//		{
			//			symbol->pSvg = itor->second;
			//		}
			//	}
			//}
		
			lineStyles.push_back(ls);
		}
	}

	if (s100AugmentedRay->GetLineStyleReference())
	{
		auto lineStylelist = pc->GetLineStyles();
		auto itorStyle = lineStylelist.find(s100AugmentedRay->GetLineStyleReference()->GetReference());

		S100_LineStyleBase *base = itorStyle->second;
		if (base->GetType() == 1)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			if (itorStyle != lineStylelist.end())
			{
				ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);

				//for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
				//{
				//	SENC_LineSymbol* symbol = *itorSymbol;
				//	auto svgsymbolManager = pc->GetSVGManager();
				//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
				//	if (itor != svgsymbolManager->m_svgMap.end())
				//	{
				//		symbol->pSvg = itor->second;
				//	}
				//	else
				//	{
				//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
				//		if (itor != svgsymbolManager->m_svgMap.end())
				//		{
				//			symbol->pSvg = itor->second;
				//		}
				//	}
				//}
				lineStyles.push_back(ls);
			}
		}
		else if (base->GetType() == 3)
		{
			S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;
			auto lineStyleList = cls->GetLineStyles();
			for (auto itor = lineStyleList.begin();
				itor != lineStyleList.end();
				itor++)
			{
				S100_LineStyle* ls = (S100_LineStyle*)*itor;
				SENC_LineStyle* sencls = new SENC_LineStyle();

				sencls->GetStyleFromS100(ls, pc);

				//for (auto itorSymbol = sencls->symbols.begin(); itorSymbol != sencls->symbols.end(); itorSymbol++)
				//{
				//	SENC_LineSymbol* symbol = *itorSymbol;
				//	auto svgsymbolManager = pc->GetSVGManager();
				//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
				//	if (itor != svgsymbolManager->m_svgMap.end())
				//	{
				//		symbol->pSvg = itor->second;
				//	}
				//	else
				//	{
				//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
				//		if (itor != svgsymbolManager->m_svgMap.end())
				//		{
				//			symbol->pSvg = itor->second;
				//		}
				//	}
				//}
				lineStyles.push_back(sencls);
			}
		}
	}

	if (s100AugmentedRay->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(s100AugmentedRay->GetLineStyle(), pc);

		//for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
		//{
		//	SENC_LineSymbol* symbol = *itorSymbol;
		//	auto svgsymbolManager = pc->GetSVGManager();
		//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
		//	if (itor != svgsymbolManager->m_svgMap.end())
		//	{
		//		symbol->pSvg = itor->second;
		//	}
		//	else
		//	{
		//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
		//		if (itor != svgsymbolManager->m_svgMap.end())
		//		{
		//			symbol->pSvg = itor->second;
		//		}
		//	}
		//}

		lineStyles.push_back(ls);
	}
}