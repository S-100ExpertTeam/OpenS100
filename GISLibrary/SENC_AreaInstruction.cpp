#include "stdafx.h"
#include "SENC_AreaInstruction.h"
#include "SENC_ColorFill.h"
#include "SENC_AreaFillReference.h"
#include "SENC_PixmapFill.h"
#include "SENC_SymbolFill.h"
#include "SENC_HatchFill.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"

#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"
#include "..\\PortrayalCatalogue\\S100_ColorFill.h"
#include "..\\PortrayalCatalogue\\S100_AreaInstruction.h"
#include "..\\PortrayalCatalogue\\S100_AreaFillReference.h"

#include "..\\GISLibrary\\R_FeatureRecord.h"

#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\S100Geometry\\SCoverage.h"

SENC_AreaInstruction::SENC_AreaInstruction()
{
	type = 3;
	areaFill = NULL;
}

SENC_AreaInstruction::~SENC_AreaInstruction()
{
	if (areaFill)
	{
		switch (areaFill->type)
		{
		case 1:
			delete (SENC_ColorFill*)areaFill;
			break;
		case 2:
			delete (SENC_AreaFillReference*)areaFill;
			break;
		case 3:
			delete (SENC_PixmapFill*)areaFill;
			break;
		case 4:
			delete (SENC_SymbolFill*)areaFill;
			break;
		case 5:
			delete (SENC_HatchFill*)areaFill;
			break;
		}
	}
}

void SENC_AreaInstruction::SetAreaFill(SENC_AreaFillBase* value)
{
	areaFill = value;
}


SENC_AreaFillBase* SENC_AreaInstruction::GetAreaFill()
{
	return areaFill;
}

// Area Instruction
void SENC_AreaInstruction::DrawInstruction(ID2D1DCRenderTarget* rt, ID2D1Factory1* pDirect2dFactory, ID2D1SolidColorBrush* brush, std::vector<ID2D1StrokeStyle1*>* strokeGroup, Scaler *scaler, PortrayalCatalogue* pc)
{
	auto surface = (SSurface*)fr->m_geometry;

	if (areaFill)
	{
		SENC_ColorFill* colorFill = nullptr;
		SENC_SymbolFill* symbolFill = nullptr;
		SENC_AreaFillReference* areaFillReference = nullptr;

		switch (areaFill->type)
		{
		case 1:
			colorFill = (SENC_ColorFill*)areaFill;
			if (colorFill->color)
			{
				D2D1_COLOR_F color;
				color.r = (float)(GetRValue(colorFill->color->RGBColor) / 255.0);
				color.g = (float)(GetGValue(colorFill->color->RGBColor) / 255.0);
				color.b = (float)(GetBValue(colorFill->color->RGBColor) / 255.0);
				color.a = 1 - colorFill->color->transparency;
				brush->SetColor(color);

				if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
				{
					auto geometry = surface->GetD2Geometry();
					if (geometry)
					{
						rt->FillGeometry(geometry, brush);
					}
				}
				else
				{
					auto geometry = surface->GetNewD2Geometry(pDirect2dFactory, scaler);
					if (geometry)
					{
						D2D1_MATRIX_3X2_F oldMatrix;
						rt->GetTransform(&oldMatrix);
						rt->SetTransform(D2D1::Matrix3x2F::Identity());
						// Apply the rotation transform to the render target.
						rt->SetTransform(
							D2D1::Matrix3x2F::Rotation(
								(float)scaler->GetRotationDegree(),
								D2D1::Point2F((float)scaler->sox, (float)scaler->soy))
						);
						rt->FillGeometry(geometry, brush);
						rt->SetTransform(oldMatrix);
						SafeRelease(&geometry);
					}
				}
			}
			break;
		case 2:
			areaFillReference = (SENC_AreaFillReference*)areaFill;
			{
				auto patternMap = &pc->GetS100PCManager()->areaFillInfo.patternMap;
				auto i = patternMap->find(areaFillReference->GetFileTitle().c_str());

				if (i != patternMap->end())
				{
					if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
					{
						auto geometry = surface->GetD2Geometry();
						if (geometry)
						{
							rt->FillGeometry(geometry, i->second->pBitmapBrush);
						}
					}
					else
					{
						auto geometry = surface->GetNewD2Geometry(pDirect2dFactory, scaler);

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
							//D2D1::Matrix3x2F bitmapTransform;
							//i->second->pBitmapBrush->GetTransform(&bitmapTransform);
							//bitmapTransform.Invert();

							auto featureMBR = fr->GetMBR();
							long sxmin = 0;
							long symin = 0;
							scaler->WorldToDevice(featureMBR.xmin, featureMBR.ymax, &sxmin, &symin);

							// Changed it to make the origin of the pattern the same.
							//i->second->pBitmapBrush->SetTransform(D2D1::Matrix3x2F::Translation((float)sxmin, (float)symin) *  D2D1::Matrix3x2F::Identity());
							i->second->pBitmapBrush->SetTransform(D2D1::Matrix3x2F::Identity());
							rt->FillGeometry(geometry, i->second->pBitmapBrush);
							rt->SetTransform(oldMatrix);
							SafeRelease(&geometry);
						}
					}
				}
			}
			break;
		case 4:
			symbolFill = (SENC_SymbolFill*)areaFill;
			if (symbolFill->symbol)
			{
				S100_SVG_D2D1_DLL::SVG* pSvg = symbolFill->symbol->pSvg;

				auto patternMap = &pc->GetS100PCManager()->areaFillInfo.patternMap;
				auto i = patternMap->find(symbolFill->GetFileTitle().c_str());

				if (i != patternMap->end())
				{
					if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
					{
						auto geometry = surface->GetD2Geometry();
						if (geometry)
						{
							rt->FillGeometry(geometry, i->second->pBitmapBrush);
						}
					}
					else
					{
						auto geometry = surface->GetNewD2Geometry(pDirect2dFactory, scaler);
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
							//D2D1::Matrix3x2F bitmapTransform;
							//i->second->pBitmapBrush->GetTransform(&bitmapTransform);
							//bitmapTransform.Invert();

							auto featureMBR = fr->GetMBR();
							long sxmin = 0;
							long symin = 0;
							scaler->WorldToDevice(featureMBR.xmin, featureMBR.ymax, &sxmin, &symin);

							// Changed it to make the origin of the pattern the same.
							//i->second->pBitmapBrush->SetTransform(D2D1::Matrix3x2F::Translation((float)sxmin, (float)symin) *  D2D1::Matrix3x2F::Identity());
							i->second->pBitmapBrush->SetTransform(D2D1::Matrix3x2F::Identity());
							rt->FillGeometry(geometry, i->second->pBitmapBrush);
							rt->SetTransform(oldMatrix);
							SafeRelease(&geometry);
						}
					}
				}
			}
			break;
		}
	}
}

void SENC_AreaInstruction::DrawInstruction(CDCRenderTarget* pRenderTarget, Scaler *scaler)
{
	if (!fr)
		return;
	if (!fr->m_geometry)
		return;

	int viewPointNum = 0;
	int partsIndex = 1;
	int lastPointIndex = -1;
	int preLoc = -1, curLoc = 0;

	long cx, cy;
	bool isBegin = true;
	bool nc = false;
	CD2DPointF p;
	CD2DPointF sp;

	int i = 0, j = 0;

	CD2DPathGeometry* pGeometry = new CD2DPathGeometry(pRenderTarget);
	pGeometry->Create(pRenderTarget);
	CD2DGeometrySink* pSink = new  CD2DGeometrySink(*pGeometry);

	// If the object is ready, draw what is in Field3 of the Presentation Library.
	SSurface* geo = (SSurface*)fr->m_geometry;

	GeoPoint cc(geo->m_pPoints[0].x, geo->m_pPoints[0].y);
	for (j = 0; j < geo->m_numPoints; j++)
	{
		scaler->WorldToDevice(geo->m_pPoints[j].x, geo->m_pPoints[j].y,
			&cx, &cy);

		if (isBegin)
		{
			p.x = (FLOAT)cx;
			p.y = (FLOAT)cy;
			pSink->BeginFigure(p, D2D1_FIGURE_BEGIN_FILLED);
			
			sp.x = p.x;
			sp.y = p.y;
			isBegin = false;
		}
		else
		{
			p.x = (FLOAT)cx;
			p.y = (FLOAT)cy;
			pSink->AddLine(p);

			if (nc)
			{
				cc.x = geo->m_pPoints[j].x;
				cc.y = geo->m_pPoints[j].y;
				nc = false;
			}

			else if (cc.x == geo->m_pPoints[j].x &&
				cc.y == geo->m_pPoints[j].y)
			{
				pSink->AddLine(sp);

				nc = true;
			}
		}
	}
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();

	SENC_ColorFill* colorFill = NULL;
	SENC_SymbolFill* symbolFill = NULL;
	switch (areaFill->type)
	{
	case 1:
		colorFill = (SENC_ColorFill*)areaFill;
		if (colorFill->color)
		{
			CD2DSolidColorBrush* pBlackBrush = new CD2DSolidColorBrush(pRenderTarget,
				D2D1::ColorF(((colorFill->color->RGBColor >> 16) & 0xff) / (FLOAT)255.,
							 ((colorFill->color->RGBColor >> 8)  & 0xff) / (FLOAT)255.,
							 ( colorFill->color->RGBColor        & 0xff) / (FLOAT)255., 
							 1 - (FLOAT)colorFill->color->transparency)
				);
			pRenderTarget->FillGeometry(pGeometry, pBlackBrush);

			delete pBlackBrush;
		}
		break;
	case 4:
		symbolFill = (SENC_SymbolFill*)areaFill;
		if (symbolFill->symbol)
		{
			S100_SVG_D2D1_DLL::SVG* pSvg = symbolFill->symbol->pSvg;
		}
		break;
	}
	

	delete pSink;
	delete pGeometry;
}

void SENC_AreaInstruction::DrawInstruction(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler)
{
	if (!fr)
		return;
	if (!fr->m_geometry)
		return;

	int viewPointNum = 0;
	int partsIndex = 1;
	int lastPointIndex = -1;
	int preLoc = -1, curLoc = 0;

	long cx, cy;
	bool isBegin = true;
	bool nc = false;
	CD2DPointF p;
	CD2DPointF sp;

	int i = 0, j = 0;

	ID2D1PathGeometry* pGeometry;
	HRESULT hr = pDXFactory->CreatePathGeometry(&pGeometry);
	ID2D1GeometrySink *pSink = NULL;
	hr = pGeometry->Open(&pSink);

	// If the object is ready, draw what is in Field 3 of the Presentation Library..
	SSurface* geo = (SSurface*)fr->m_geometry;

	GeoPoint cc(geo->m_pPoints[0].x, geo->m_pPoints[0].y);
	for (j = 0; j < geo->m_numPoints; j++)
	{
		scaler->WorldToDevice(geo->m_pPoints[j].x, geo->m_pPoints[j].y,
			&cx, &cy);

		if (isBegin)
		{
			p.x = (FLOAT)cx;
			p.y = (FLOAT)cy;
			pSink->BeginFigure(p, D2D1_FIGURE_BEGIN_FILLED);
			
			sp.x = p.x;
			sp.y = p.y;
			isBegin = false;
		}
		else
		{
			p.x = (FLOAT)cx;
			p.y = (FLOAT)cy;
			pSink->AddLine(p);

			if (nc)
			{
				cc.x = geo->m_pPoints[j].x;
				cc.y = geo->m_pPoints[j].y;
				nc = false;
			}

			else if (cc.x == geo->m_pPoints[j].x &&
				cc.y == geo->m_pPoints[j].y)
			{
				pSink->AddLine(sp);

				nc = true;
			}
		}
	}
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();

	SENC_ColorFill* colorFill = NULL;
	SENC_SymbolFill* symbolFill = NULL;
	switch (areaFill->type)
	{
	case 1:
		colorFill = (SENC_ColorFill*)areaFill;
		if (colorFill->color)
		{
			ID2D1SolidColorBrush* pBlackBrush;
			
			if (S_OK == pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(((colorFill->color->RGBColor >> 16) & 0xff) / (FLOAT)255.,
							 ((colorFill->color->RGBColor >> 8) & 0xff) / (FLOAT)255.,
							  (colorFill->color->RGBColor & 0xff) / (FLOAT)255.,
							1 - (FLOAT)colorFill->color->transparency),
				&pBlackBrush))
			{
				pRenderTarget->FillGeometry(pGeometry, pBlackBrush);
			}
			pBlackBrush->Release();
		}
		break;
	case 4:
		symbolFill = (SENC_SymbolFill*)areaFill;
		if (symbolFill->symbol)
		{
			S100_SVG_D2D1_DLL::SVG* pSvg = symbolFill->symbol->pSvg;
		}
		break;
	}
	

	pSink->Release();
	pGeometry->Release();
}

void SENC_AreaInstruction::ChangePallete(PortrayalCatalogue *pc)
{
	auto areaFill = GetAreaFill();
	if (areaFill)
	{
		areaFill->ChangePallete(pc);
	}
}

void SENC_AreaInstruction::FromS100Instruction(
	S100_Instruction* s100Instruction,
	PortrayalCatalogue *pc,
	PCOutputSchemaManager* output)
{
	output->GetSENCFromS100Common(s100Instruction, this);

	SENC_ColorFill *colorFill = NULL;
	SENC_SymbolFill *symbolFill = NULL;
	SENC_AreaFillReference *areaFillReference = NULL;
	SENC_PixmapFill *pixmapFill = NULL;
	SENC_HatchFill *hatchFill = NULL;

	auto s100AreaInstruction = (S100_AreaInstruction*)s100Instruction;

	if (s100AreaInstruction->GetAreaFill())
	{
		switch (s100AreaInstruction->GetAreaFill()->GetType())
		{
		case 1: // Color Fill
			areaFill = new SENC_ColorFill();

			colorFill = (SENC_ColorFill*)areaFill;
			if (((S100_ColorFill*)s100AreaInstruction->GetAreaFill())->GetColor())
			{
				colorFill->color = new SENC_Color();
				colorFill->color->SetToken(((S100_ColorFill*)s100AreaInstruction->GetAreaFill())->GetColor()->GetToken());
				colorFill->color->SetTransparency((float)_wtof(((S100_ColorFill*)s100AreaInstruction->GetAreaFill())->GetColor()->GetTransparency().c_str()));

				auto colorProfile = pc->GetColorProfile();
				if (colorProfile)
				{
					colorFill->color->RGBColor = colorProfile->GetRGBRef(pc->GetCurrentPaletteName(), colorFill->color->token);;
				}
			}
			break;
		case 2:
		{
			areaFill = new SENC_AreaFillReference();
			auto s100AreaFillReference = (S100_AreaFillReference*)s100AreaInstruction->GetAreaFill();
			areaFill->SetFileTitle(s100AreaFillReference->GetReference());
		}
		break;
		case 4: // Symbol Fill
			areaFill = new SENC_SymbolFill();
			symbolFill = (SENC_SymbolFill*)areaFill;
			symbolFill->areaCRSType = SENC_CommonFuc::GetAreaCRSType(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetAreaCRS());

			areaFill->SetFileTitle(s100AreaInstruction->GetAreaFill()->GetFileTitle());

			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol())
			{
				symbolFill->symbol = new SENC_Symbol();
				symbolFill->symbol->reference = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol()->GetReference();
				if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol()->GetRotation())
				{
					symbolFill->symbol->rotation = new SENC_Rotation();
					symbolFill->symbol->rotation->useValueOf = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol()->GetRotation()->GetUseValueOf();
					symbolFill->symbol->rotation->value = _wtoi(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol()->GetRotation()->GetValue().c_str());
				}
				symbolFill->symbol->rotationCRS = SENC_CommonFuc::GetRotationCRS(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol()->GetRotationCRS());
				symbolFill->symbol->scaleFactor = (float)(_wtof(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetSymbol()->GetScaleFactor().c_str()));

				auto svgSymbolManger = pc->GetSVGManager();
				auto itor = svgSymbolManger->m_svgMap.find(symbolFill->symbol->reference);
				if (itor != svgSymbolManger->m_svgMap.end())
				{
					symbolFill->symbol->pSvg = itor->second;
				}
				else
				{

					itor = svgSymbolManger->m_svgMap.find(L"QUESMRK1");
					if (itor != svgSymbolManger->m_svgMap.end())
					{
						symbolFill->symbol->pSvg = itor->second;
					}
				}
			}
			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1())
			{
				symbolFill->v1.x = _wtof(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1()->GetX().c_str());
				symbolFill->v1.y = _wtof(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1()->GetY().c_str());
			}
			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2())
			{
				symbolFill->v2.x = _wtof(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2()->GetX().c_str());
				symbolFill->v2.y = _wtof(((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2()->GetY().c_str());
			}
		}
	}
}