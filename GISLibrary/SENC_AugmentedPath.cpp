#include "stdafx.h"
#include "GISLibrary.h"
#include "D2D1Resources.h"
#include "SENC_AugmentedPath.h"
#include "SENC_LineStyleReference.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_LineStyle.h"
#include "PCOutputSchemaManager.h"
#include "R_FeatureRecord.h"
#include "SENC_CommonFuc.h"

#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\PortrayalCatalogue\\S100_AugmentedPath.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

SENC_AugmentedPath::SENC_AugmentedPath()
{
	path = NULL;
	type = 8;
}

SENC_AugmentedPath::~SENC_AugmentedPath()
{
	if (path)
		delete path;

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

}

#pragma warning(disable:4244)
void SENC_AugmentedPath::DrawInstruction(ID2D1DCRenderTarget* rt, ID2D1Factory1* factory, ID2D1SolidColorBrush* brush, std::vector<ID2D1StrokeStyle1*>* strokeGroup, Scaler *scaler, PortrayalCatalogue* pc)
{
	if (!fr)
		return;
	if (!fr->m_geometry)
		return;

	if (!path)
	{
		return;
	}

	if (fr->m_geometry->type == 1)
	{
		SPoint* p = (SPoint*)fr->m_geometry;
		scaler->WorldToDevice(p->m_point.x, p->m_point.y, &SGeometry::viewPoints[0].x, &SGeometry::viewPoints[0].y);

		for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
		{
			SENC_LineStyleBase *lineStyle = *itor;
			if (lineStyle->type == 1)
			{
				SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;

				if (ls->geometryVector.size() == 0)
				{
					for (auto itor = path->arcByRadiuses.begin(); itor != path->arcByRadiuses.end(); itor++)
					{
						SENC_ArcByRadius* _arcByRadius = *itor;
						ID2D1PathGeometry *pGeometry = nullptr;

						HRESULT hr = factory->CreatePathGeometry(&pGeometry);
						ID2D1PathGeometry *pathGeo = (ID2D1PathGeometry*)pGeometry;
						ID2D1GeometrySink* pSink = nullptr;
						hr = pathGeo->Open(&pSink);

						FLOAT displayRadius = _arcByRadius->radius * 3.5;

						D2D1_POINT_2F startPoint = { 0.0F, displayRadius };
						D2D1_POINT_2F endPoint = { 0.0F, displayRadius };

						float startAngle = _arcByRadius->sector->startAngle + 180;
						float endAngle = startAngle + _arcByRadius->sector->angularDistance;

						D2D1::Matrix3x2F matrix1 = D2D1::Matrix3x2F::Identity();
						matrix1 = matrix1.Rotation(startAngle);
						startPoint = matrix1.TransformPoint(startPoint);

						D2D1::Matrix3x2F matrix2 = D2D1::Matrix3x2F::Identity();
						matrix2 = matrix2.Rotation(endAngle);
						endPoint = matrix2.TransformPoint(endPoint);

						pSink->BeginFigure(
							startPoint,
							D2D1_FIGURE_BEGIN_HOLLOW);

						pSink->AddArc(
							D2D1::ArcSegment(
								endPoint,
								D2D1::SizeF(displayRadius, displayRadius),
								0,
								D2D1_SWEEP_DIRECTION_CLOCKWISE,
								(endAngle - startAngle) > 180 ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL
							)
						);

						pSink->EndFigure(D2D1_FIGURE_END_OPEN);

						hr = pSink->Close();

						SafeRelease(&pSink);

						ls->geometryVector.push_back(pGeometry);
					}
				}

				D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(SGeometry::viewPoints[0].x, SGeometry::viewPoints[0].y);

				rt->SetTransform(trans);

				for (auto iter = ls->geometryVector.begin(); iter != ls->geometryVector.end(); iter++)
				{
					auto pGeometry = *iter;

					brush->SetColor(D2D1::ColorF((FLOAT)(GetRValue(ls->pen_color) / 255.0), (FLOAT)(GetGValue(ls->pen_color) / 255.0), (FLOAT)(GetBValue(ls->pen_color) / 255.0)));

					if (ls->dashes.size() > 0)
					{
						float dashes[] = {
							(float)(ls->dashes.front()->length + 0.01) / (float)0.32,
							(float)(ls->intervalLength - ls->dashes.front()->length + 0.01) / (float)0.32
						};
						ID2D1StrokeStyle1* dash = nullptr;
						gisLib->D2.pD2Factory->CreateStrokeStyle(
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

						rt->DrawGeometry(pGeometry, brush, ls->pen_width / 0.32, dash);
						SafeRelease(&dash);
					}
					else
					{
						rt->DrawGeometry(pGeometry, brush, ls->pen_width / 0.32, (*strokeGroup)[0]);
					}
				}
			}
			else if (lineStyle->type == 2)
			{
			}
			else if (lineStyle->type == 3)
			{
			}
		}
	}
}

void SENC_AugmentedPath::ChangePallete(PortrayalCatalogue *pc)
{
	for (auto i = lineStyles.begin(); i != lineStyles.end(); i++)
	{
		(*i)->ChangePallete(pc);
	}
}

void SENC_AugmentedPath::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100AugmentedPath = (S100_AugmentedPath*)s100Instruction;
	output->GetSENCFromS100Common(s100Instruction, this);

	areaCRSType = SENC_CommonFuc::GetAreaCRSType(s100AugmentedPath->GetCrsType());

	if (s100AugmentedPath->GetPath())
	{
		path = new SENC_Path();
		auto poly = s100AugmentedPath->GetPath()->GetPolylines();
		for (auto itor = poly.begin(); itor != poly.end(); itor++)
		{

		}

		auto arc3 = s100AugmentedPath->GetPath()->GetArc3Pointses();
		for (auto itor = arc3.begin(); itor != arc3.end(); itor++)
		{

		}

		auto arcBy = s100AugmentedPath->GetPath()->GetArcByRadiuses();
		for (auto itor = arcBy.begin(); itor != arcBy.end(); itor++)
		{
			S100_ArcByRadius* p = &(*itor);
			SENC_ArcByRadius* sp = new SENC_ArcByRadius();
			sp->center.x = p->GetCenter()->GetX();
			sp->center.y = p->GetCenter()->GetY();
			sp->radius = _wtof(p->GetRadius().c_str());
			if (p->GetSector())
			{
				sp->sector = new SENC_Sector();
				sp->sector->angularDistance = _wtof(p->GetSector()->GetAnglearDistance().c_str());
				sp->sector->startAngle = _wtof(p->GetSector()->GetStartAngle().c_str());
			}
			path->arcByRadiuses.push_back(sp);
		}
		auto annul = s100AugmentedPath->GetPath()->GetAnnuluses();
		for (auto itor = annul.begin(); itor != annul.end(); itor++)
		{

		}
	}

	if (s100AugmentedPath->GetCompositeLineStyle())
	{
		auto lineStyleslist = s100AugmentedPath->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = lineStyleslist.begin();
			itor != lineStyleslist.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);

			for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
			{
				SENC_LineSymbol* symbol = *itorSymbol;
				auto svgSymbolManager = pc->GetSVGManager();
				auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
				else
				{
					itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
				}
			}
			lineStyles.push_back(ls);
		}
	}

	if (s100AugmentedPath->GetLineStyleReference())
	{
		auto lineStylelist = pc->GetLineStyles();
		auto itorStyle = lineStylelist.find(s100AugmentedPath->GetLineStyleReference()->GetReference());

		S100_LineStyleBase *base = itorStyle->second;
		if (base->GetType() == 1)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			if (itorStyle != lineStylelist.end())
			{
				ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);

				for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
				{
					SENC_LineSymbol* symbol = *itorSymbol;
					auto svgSymbolManager = pc->GetSVGManager();
					auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
					else
					{
						itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
						if (itor != svgSymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
					}
				}
				lineStyles.push_back(ls);
			}
		}
		else if (base->GetType() == 3) {

			S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;

			auto lineStylesList = cls->GetLineStyles();


			for (auto itor = lineStylesList.begin();
				itor != lineStylesList.end();
				itor++)
			{
				S100_LineStyle* ls = *itor;
				SENC_LineStyle* sencls = new SENC_LineStyle();

				sencls->GetStyleFromS100(ls, pc);

				for (auto itorSymbol = sencls->symbols.begin(); itorSymbol != sencls->symbols.end(); itorSymbol++)
				{
					SENC_LineSymbol* symbol = *itorSymbol;
					auto svgsymbolManager = pc->GetSVGManager();
					auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
					if (itor != svgsymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
					else
					{
						itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
						if (itor != svgsymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
					}
				}
				lineStyles.push_back(sencls);
			}
		}
	}

	if (s100AugmentedPath->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(s100AugmentedPath->GetLineStyle(), pc);

		for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
		{
			SENC_LineSymbol* symbol = *itorSymbol;
			auto svgSymbolManager = pc->GetSVGManager();
			auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
			if (itor != svgSymbolManager->m_svgMap.end())
			{
				symbol->pSvg = itor->second;
			}
			else
			{
				itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
			}
		}
		lineStyles.push_back(ls);
	}
}