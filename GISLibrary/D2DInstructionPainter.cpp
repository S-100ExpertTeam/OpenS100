#include "stdafx.h"
#include "D2DInstructionPainter.h"
#include "D2DConvert.h"
#include "D2DGeometryCache.h"
#include "D2DInstructionCache.h"
#include "D2DView.h"
#include "D2D1Resources.h"
#include "PortrayalRenderCache.h"
#include "GISLibrary.h"
#include "R_FeatureRecord.h"
#include "SCommonFuction.h"
#include "SCompositeCurve.h"
#include "SCoverage.h"
#include "SCurve.h"
#include "SPoint.h"
#include "SENC_AreaFillReference.h"
#include "SENC_AreaInstruction.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AugmentedRay.h"
#include "SENC_ColorFill.h"
#include "SENC_CommonFuc.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_Define.h"
#include "SENC_HatchFill.h"
#include "SENC_LineInstruction.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"
#include "SENC_PointInstruction.h"
#include "SENC_SpatialReference.h"
#include "SENC_SymbolFill.h"
#include "SENC_TextInstruction.h"
#include "SENC_pixmapFill.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../LatLonUtility/LatLonUtility.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../S100Engine/AreaPatternBitmap.h"
#include "../S100Engine/S100PCManager.h"
#include "../S100Engine/S100Render.h"

// The bodies below were member functions of the instruction classes (DrawInstruction) and two
// loops of LayerManager::AddSymbolDrawing. They are kept as they were: each function first names
// the members of the instruction the old body used, then the old body follows.

// As in the files the bodies came from.
#pragma warning(disable:4244)
#pragma warning(disable:4838)

namespace
{
	// Millimetres of an augmented ray to pixels.
	const float factor = 3.5;
}

namespace GISLibrary
{
	D2DInstructionPainter::D2DInstructionPainter(D2D1Resources* d2, Scaler* scaler, PortrayalCatalogue* pc)
		: d2(d2), scaler(scaler), pc(pc)
	{
	}

	void D2DInstructionPainter::DrawLineStyle(SENC_LineStyleBase* lineStyle, SCurve* curve)
	{
		// A composite style visits its parts with the same curve.
		const auto outer = styleCurve;
		styleCurve = curve;
		lineStyle->Accept(*this);
		styleCurve = outer;
	}

	void D2DInstructionPainter::Visit(SENC_CompositeLineStyle& lineStyle)
	{
		for (auto i = lineStyle.lineStyles.begin(); i != lineStyle.lineStyles.end(); i++)
		{
			DrawLineStyle(*i, styleCurve);
		}
	}
}

// ---- SENC_AreaInstruction::DrawInstruction
void GISLibrary::D2DInstructionPainter::Visit(SENC_AreaInstruction& instruction)
{
	auto& fr = instruction.fr;
	auto& areaFill = instruction.areaFill;

	auto rt = d2->RenderTarget();
	auto pDirect2dFactory = d2->Factory();
	auto brush = d2->SolidColorBrush();
	if (!brush || !pDirect2dFactory || !rt)
	{
		return;
	}

	rt->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	auto surface = (SSurface*)fr->GetGeometry();

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
				D2D1_COLOR_F color = GISLibrary::ToD2D(GISLibrary::PortrayalRender::Color(pc, colorFill->color->token));
				
				color.a = 1 - colorFill->color->transparency;
				brush->SetColor(color);

				if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
				{
					auto geometry = GISLibrary::D2DGeometry::Get(surface, pDirect2dFactory);
					
					if (geometry)
					{
						rt->FillGeometry(geometry, brush);
					}
				}
				else
				{
					auto geometry = GISLibrary::D2DGeometry::CreateScaled(surface, pDirect2dFactory, scaler);
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
				auto patternMap = &GISLibrary::PortrayalRender::Caches(pc)->areaFillInfo.patternMap;
				auto i = patternMap->find(areaFillReference->GetFileStem().c_str());

				if (i == patternMap->end())
				{
					i = patternMap->find(L"QUESMRK1");
				}

				if (i != patternMap->end())
				{
					if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
					{
						auto geometry = GISLibrary::D2DGeometry::Get(surface, pDirect2dFactory);
						if (geometry)
						{
							rt->FillGeometry(geometry, i->second->pBitmapBrush);
						}
					}
					else
					{
						auto geometry = GISLibrary::D2DGeometry::CreateScaled(surface, pDirect2dFactory, scaler);

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

							auto featureMBR = surface->GetMBR();
							long sxmin = 0;
							long symin = 0;
							scaler->WorldToDevice(featureMBR.xmin, featureMBR.ymax, &sxmin, &symin);

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
			//if (symbolFill->symbol)			
			{
				auto patternMap = &GISLibrary::PortrayalRender::Caches(pc)->areaFillInfo.patternMap;
				auto i = patternMap->find(symbolFill->GetFileStem().c_str());

				ID2D1Brush* imageBrush = nullptr;
				AreaPatternBitmap* bitmapImage = nullptr;

				// Symbol Fill을 별도의 파일(AreaFills)에서 읽은 경우
				if (symbolFill->GetFileStem().empty() == false)
				{
					if (i != patternMap->end())
					{
						imageBrush = i->second->pBitmapBrush;
					}
				}
				else
				{
					auto _areaFill = symbolFill->as_AreaFill();
					bitmapImage = GISLibrary::PortrayalRender::Caches(pc)->CreateBitmapImage(_areaFill.get(), pDirect2dFactory, d2->pImagingFactory, d2->SolidStrokeStyle());
					if (bitmapImage)
					{
						bitmapImage->pBitmapBrush = GISLibrary::PortrayalRender::Caches(pc)->CreateBitmapBrush(bitmapImage, rt);
						imageBrush = bitmapImage->pBitmapBrush;
						imageBrush->SetTransform(GISLibrary::D2DView::InverseMatrix(*scaler));
					}
				}

				if (imageBrush != nullptr)
				{
					if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
					{
						auto geometry = GISLibrary::D2DGeometry::Get(surface, pDirect2dFactory);
						if (geometry)
						{
							rt->FillGeometry(geometry, imageBrush);
						}
					}
					else
					{
						auto geometry = GISLibrary::D2DGeometry::CreateScaled(surface, pDirect2dFactory, scaler);
						if (geometry)
						{
							D2D1_MATRIX_3X2_F oldMatrix;
							rt->GetTransform(&oldMatrix);
							rt->SetTransform(D2D1::Matrix3x2F::Identity());
							imageBrush->SetTransform(D2D1::Matrix3x2F::Identity());
							rt->FillGeometry(geometry, imageBrush);
							rt->SetTransform(oldMatrix);
							SafeRelease(&geometry);
						}
					}
				}

				if (bitmapImage)
				{
					delete bitmapImage;
					bitmapImage = nullptr;
				}
			}
			break;
		}
	}

	rt->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

// ---- SENC_LineInstruction::DrawInstruction
void GISLibrary::D2DInstructionPainter::Visit(SENC_LineInstruction& instruction)
{
	auto& fr = instruction.fr;
	auto& lineStyles = instruction.lineStyles;
	auto& spatialReference = instruction.spatialReference;


	auto geom = fr->GetGeometry();

	std::list<SCurve*> curListCurveLink;

	// Surface
	if (geom->GetType() == SGeometryType::Surface)
	{
		auto surface = (SSurface*)geom;
		surface->GetCurveList(curListCurveLink);
	}
	// Composite Curve
	else if (geom->GetType() == SGeometryType::CompositeCurve)
	{
		auto compositeCurve = (SCompositeCurve*)geom;
		compositeCurve->GetCurveList(curListCurveLink);
	}
	// Curve
	else if (geom->GetType() == SGeometryType::Curve)
	{
		auto curve = (SCurve*)geom;
		for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
		{
			SENC_LineStyleBase* lineStyleBase = *itor;

			if (curve->isDraw())
			{
				DrawLineStyle(lineStyleBase, curve);
			}
			
		}
		return;
	}

	if (curListCurveLink.size() == 0)
	{
		return;
	}

	if (spatialReference.size() == 0)
	{
		for (auto i = curListCurveLink.begin(); i != curListCurveLink.end(); i++)
		{
			auto curve = (*i);

			if (curve->isDraw())
			{
				for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
				{
					SENC_LineStyleBase* lineStyleBase = *itor;
					DrawLineStyle(lineStyleBase, *i);
				}
			}
		}
	}

	//rt->SetTransform(scaler->GetMatrix());
	for (auto i = spatialReference.begin(); i != spatialReference.end(); i++)
	{
		for (auto j = curListCurveLink.begin(); j != curListCurveLink.end(); j++)
		{
			if ((*i)->reference == (*j)->GetRCID())
			{
				if ((*j)->isDraw())
				{
					for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
					{
						SENC_LineStyleBase* lineStyleBase = *itor;
						DrawLineStyle(lineStyleBase, *j);
					}
				}
				break;
			}
		}
	}
}

// ---- SENC_LineStyle::DrawInstruction
void GISLibrary::D2DInstructionPainter::Visit(SENC_LineStyle& lineStyle)
{
	auto curveHasOrient = styleCurve;
	auto rt = d2->RenderTarget();
	auto pDirect2dFactory = d2->Factory();
	auto brush = d2->SolidColorBrush();
	auto strokeGroup = &d2->D2D1StrokeStyleGroup;
	auto& dashes = lineStyle.dashes;
	auto& symbols = lineStyle.symbols;
	auto& intervalLength = lineStyle.intervalLength;
	auto& pen_width = lineStyle.pen_width;

	auto curve = curveHasOrient;
	auto name = lineStyle.GetLineStyleName();

	if (name != L"")
	{
		auto numPoints = curve->GetNumPoints();
		auto d2Points = new D2D1_POINT_2F[numPoints];

		for (int pi = 0; pi < numPoints; pi++)
		{
			scaler->WorldToDevice_F(
				curve->GetX(pi),
				curve->GetY(pi),
				&d2Points[pi].x,
				&d2Points[pi].y);
		}

		auto s100PCManager = GISLibrary::PortrayalRender::Caches(pc);
		if (s100PCManager)
		{
			IWICBitmap* pIWICBitmap = s100PCManager->GetLineImage(lineStyle.GetLineStyleName() + _T(".xml"));

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
					//OutputDebugString(_T("Failed to create bitmap\n"));
				}
				else
				{
					//D2D1_MATRIX_3X2_F oldMatrix;
					//rt->GetTransform(&oldMatrix);
					//rt->SetTransform(D2D1::Matrix3x2F::Identity());
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

					S100Render::DrawBitmapOnPolyline(rt, d2Points, numPoints, pBitmap, GISLibrary::D2DView::Rect(*scaler), GISLibrary::ToD2D(GISLibrary::PortrayalRender::Color(pc, lineStyle.GetPenColorToken())), strokeGroup->at(1));
					SafeRelease(&pBitmap);
				}
			}
		}

		delete[] d2Points;
		d2Points = nullptr;
	}
	else
	{
		auto PEN_COLOR = GISLibrary::ToD2D(GISLibrary::PortrayalRender::Color(pc, lineStyle.GetPenColorToken()));
		auto PEN_WIDTH = (float)((pen_width + 0.01) / 0.32);

		D2D1_COLOR_F color = PEN_COLOR;
		color.a = 1;
		brush->SetColor(color);

		if (dashes.size() > 0)
		{
			float _dashes[] = { (float)(dashes.front()->length + 0.01) / (float)(0.32 * PEN_WIDTH), (float)(intervalLength - dashes.front()->length + 0.01) / (float)(0.32 * PEN_WIDTH) };
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
				_dashes,
				ARRAYSIZE(_dashes),
				&dash
			);

			if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
			{
				auto geometry = GISLibrary::D2DGeometry::Get(curve, pDirect2dFactory);
				if (geometry)
				{
					rt->DrawGeometry(geometry, brush, PEN_WIDTH, dash);
				}
			}
			else
			{
				auto geometry = GISLibrary::D2DGeometry::CreateScaled(curve, pDirect2dFactory, scaler);
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
				auto geometry = GISLibrary::D2DGeometry::Get(curve, pDirect2dFactory);
				if (geometry)
				{
					rt->DrawGeometry(geometry, brush, PEN_WIDTH, (*strokeGroup)[0]);
				}
			}
			else
			{
				//Memory leak
				auto geometry = GISLibrary::D2DGeometry::CreateScaled(curve, pDirect2dFactory, scaler);
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

// ---- SENC_LineStyleReference::DrawInstruction
void GISLibrary::D2DInstructionPainter::Visit(SENC_LineStyleReference& lineStyle)
{
	auto curveHasOrient = styleCurve;
	auto rt = d2->RenderTarget();

	//auto curve = curveHasOrient->GetCurve();
	auto curve = curveHasOrient;

	auto numPoints = curve->GetNumPoints();

	auto d2Points = new D2D1_POINT_2F[numPoints];

	for (int pi = 0; pi < numPoints; pi++)
	{
		scaler->WorldToDevice_F(
			curve->GetX(pi),
			curve->GetY(pi),
			&d2Points[pi].x,
			&d2Points[pi].y);
	}

	auto s100PCManager = GISLibrary::PortrayalRender::Caches(pc);

	if (s100PCManager)
	{
		IWICBitmap* pIWICBitmap = s100PCManager->GetLineImage(lineStyle.GetReference() + _T(".xml"));

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
				//OutputDebugString(_T("Failed to create bitmap\n"));
			}
			else
			{
				//pc->GetS100Render().DrawBitmapOnPolyline(rt, d2Points, numPoints, pBitmap, scaler->GetD2Rect());
				S100Render::DrawBitmapOnPolyline(rt, d2Points, numPoints, pBitmap, GISLibrary::D2DView::Rect(*scaler));
				SafeRelease(&pBitmap);
			}
		}
	}

	delete[] d2Points;
	d2Points = nullptr;
}

// ---- SENC_AugmentedRay::DrawInstruction
void GISLibrary::D2DInstructionPainter::Visit(SENC_AugmentedRay& instruction)
{
	auto& fr = instruction.fr;
	auto& lineStyles = instruction.lineStyles;
	auto& direction = instruction.direction;
	auto& length = instruction.length;

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
					// The direction is from north, which is not up about a pole.
					const double onScreen = direction + NorthAngle(mx, my) - 90;
					ix = pBeginningPt.x + (int)((cos(onScreen * M_PI / 180) * length) * factor);
					iy = pBeginningPt.y + (int)((sin(onScreen * M_PI / 180) * length) * factor);

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

				ID2D1PathGeometry* pGeometry = nullptr;
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
				
				brush->SetColor(GISLibrary::ToD2D(GISLibrary::PortrayalRender::Color(pc, ls->GetPenColorToken())));

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

// ---- SENC_AugmentedPath::DrawInstruction
void GISLibrary::D2DInstructionPainter::Visit(SENC_AugmentedPath& instruction)
{
	auto& fr = instruction.fr;
	auto& path = instruction.path;
	auto& lineStyles = instruction.lineStyles;

	auto factory = d2->Factory();
	auto rt = d2->RenderTarget();
	auto brush = d2->SolidColorBrush();
	auto stroke = d2->SolidStrokeStyle();
	if (!factory || !rt || !brush || !stroke)
	{
		return;
	}

	if (!fr) {
		return;
	}

	auto geom = fr->GetGeometry();

	if (!fr->GetGeometry()) {
		return;
	}

	if (!path) {
		return;
	}

	if (geom->GetType() == SGeometryType::Point)
	{
		SPoint* p = (SPoint*)geom;
		scaler->WorldToDevice(p->x, p->y, &SGeometry::viewPoints[0].x, &SGeometry::viewPoints[0].y);

		for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
		{
			SENC_LineStyleBase *lineStyle = *itor;
			if (lineStyle->type == 1)
			{
				SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;

				auto& arcs = GISLibrary::D2DInstructionCache::ArcGeometries(*ls);
				if (arcs.size() == 0)
				{
					for (auto _itor = path->arcByRadiuses.begin(); _itor != path->arcByRadiuses.end(); _itor++)
					{
						SENC_ArcByRadius* _arcByRadius = *_itor;
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

						arcs.push_back(pGeometry);
					}
				}

				// The arcs are built from north up: turned to where north is at the point.
				D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Rotation(static_cast<FLOAT>(NorthAngle(p->x, p->y)))
					* D2D1::Matrix3x2F::Translation(SGeometry::viewPoints[0].x, SGeometry::viewPoints[0].y);

				rt->SetTransform(trans);

				for (auto iter = arcs.begin(); iter != arcs.end(); iter++)
				{
					auto pGeometry = *iter;

					//brush->SetColor(D2D1::ColorF((FLOAT)(GetRValue(ls->pen_color) / 255.0), (FLOAT)(GetGValue(ls->pen_color) / 255.0), (FLOAT)(GetBValue(ls->pen_color) / 255.0)));
					brush->SetColor(GISLibrary::ToD2D(GISLibrary::PortrayalRender::Color(pc, ls->GetPenColorToken())));

					if (ls->dashes.size() > 0)
					{
						float dashes[] = {
							(float)(ls->dashes.front()->length + 0.01) / (float)0.32,
							(float)(ls->intervalLength - ls->dashes.front()->length + 0.01) / (float)0.32
						};
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

						rt->DrawGeometry(pGeometry, brush, ls->pen_width / 0.32, dash);
						SafeRelease(&dash);
					}
					else
					{
						rt->DrawGeometry(pGeometry, brush, ls->pen_width / 0.32, stroke);
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

// ---- LayerManager::AddSymbolDrawing, the body of the loop over point instructions
void GISLibrary::D2DInstructionPainter::Visit(SENC_PointInstruction& target)
{
	auto D2 = d2;
	auto instruction = &target;

	points.clear();

	//if (ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE)
	//{
	//	instruction->GetDrawPointsDynamic(scaler, points);
	//}
	//else
	//{
	//	instruction->GetDrawPoints(scaler, points);
	//}
	instruction->GetDrawPoints(scaler, points);

	for (auto pi = points.begin(); pi != points.end(); pi++)
	{
		// A rotation from north (GeographicCRS) starts from where north is at the point,
		// which is not up about a pole.
		float northAngle = 0;
		if (IsPolarProjection())
		{
			double mx = 0;
			double my = 0;
			scaler->DeviceToWorld(static_cast<long>(pi->x), static_cast<long>(pi->y), &mx, &my);
			northAngle = static_cast<float>(NorthAngle(mx, my));
		}

		// rotate Point
		if (scaler->GetRotationDegree())
		{
			double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

			FLOAT tempX = (float)(scaler->soy - pi->y) * (float)sin(radian) + (float)(scaler->sox - pi->x) * (float)cos(radian);
			FLOAT tempY = (float)(scaler->soy - pi->y) * (float)cos(radian) - (float)(scaler->sox - pi->x) * (float)sin(radian);

			pi->x = tempX + (float)scaler->sox;
			pi->y = tempY + (float)scaler->soy;
		}

		float rotation = 0;

		if ((instruction->symbol)->rotation)
		{
			rotation = (float)(instruction->symbol)->rotation;
		}
		if (instruction->symbol->rotationCRS == GEOGRAPHIC_CRS)
		{
			rotation += northAngle;
		}


		if (instruction && instruction->symbol)
		{
			auto s100PCManager = GISLibrary::PortrayalRender::Caches(pc);
			if (s100PCManager)
			{
				auto pRenderTarget = D2->RenderTarget();

				D2D1::Matrix3x2F oldTransform;
				pRenderTarget->GetTransform(&oldTransform);

				s100PCManager->Draw(
					pugi::as_utf8(instruction->symbol->reference),
					pRenderTarget,
					D2->SolidColorBrush(),
					D2->SolidStrokeStyle(),
					D2D1::Point2F(pi->x, pi->y),
					rotation,
					5);

				pRenderTarget->SetTransform(oldTransform);
			}
		}
	}
}

// ---- LayerManager::AddSymbolDrawing, the body of the loop over text instructions
void GISLibrary::D2DInstructionPainter::Visit(SENC_TextInstruction& target)
{
	auto D2 = d2;
	auto instruction = &target;
	D2D1_SIZE_F renderTargetSize = D2->pRT->GetSize();

	points.clear();

	//if (ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE)
	//{
	//	instruction->GetDrawPointsDynamic(scaler, points);
	//}
	//else
	//{
	//	instruction->GetDrawPoints(scaler, points);
	//}
	instruction->GetDrawPoints(scaler, points);

	SENC_TextPoint* textPoint = instruction->textPoint;
	auto itorTp = textPoint->elements.begin();

	if (itorTp != textPoint->elements.end())
	{
		SENC_Element* element = *itorTp;

		std::wstring viewString = element->text.value;

		int angle = element->font.slant;
		double radian = 0;
		if (angle)
		{
			radian = -angle / 180. * M_PI;
		}

		int bodySize = (int)(element->bodySize * (float)1.358);

		IDWriteTextFormat* useWTF = nullptr;

		if (element->font.isUpright())
		{
			useWTF = D2->getWriteTextFormat(bodySize);
		}
		else
		{
			useWTF = D2->getSlantWriteTextFormat(bodySize);
		}

		useWTF->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

		auto textMetrics = D2DInstructionCache::TextMetrics(*instruction);
		if (!textMetrics)
		{
			IDWriteTextLayout* textLayout = nullptr;
			float caretX = renderTargetSize.width;
			float caretY = renderTargetSize.height;

			const wchar_t* vText = element->text.value.c_str();
			int vTextSize = (int)element->text.value.size();
			D2->pDWriteFactory->CreateTextLayout(
				vText,
				vTextSize,
				useWTF,
				caretX,
				caretY,
				&textLayout
			);

			textMetrics = &D2DInstructionCache::NewTextMetrics(*instruction);
			textLayout->GetMetrics(textMetrics);

			SafeRelease(&textLayout);
		}

		float width = textMetrics->width;
		float height = textMetrics->height;

		float offset_x = 0;
		float offset_y = 0;

		//// INCH to mm
/*				float offsetUnitX = (dpiX / (float)25.4);
		float offsetUnitY = (dpiY / (float)25.4);*/

		// Determine the size of the Offset
		//float XOFFS = ((float)textPoint->offset.x * offsetUnitX);
		//float YOFFS = -((float)textPoint->offset.y * offsetUnitY);
		float XOFFS = (float)(((float)textPoint->offset.x / 0.32) * 1.358);
		float YOFFS = (float)(-((float)textPoint->offset.y / 0.32) * 1.358);

		// HJUST 
		// CENTRE
		if (textPoint->horizontalAlignment == CENTER)
		{
			offset_x = -width / (float)2.;
		}
		// RIGHT
		else if (textPoint->horizontalAlignment == END)
		{
			offset_x = -width;
		}
		// LEFT
		else if (textPoint->horizontalAlignment == START)
		{
			offset_x = 0;
		}
		else
		{
			offset_x = 0;
		}

		// VJUST
		// BOTTOM
		if (textPoint->verticalAlignment == BOTTOM)
		{
			offset_y = -height;
		}
		// CENTRE
		else if (textPoint->verticalAlignment == CENTER)
		{
			offset_y = -height / (float)2.;
		}
		// TOP
		else if (textPoint->verticalAlignment == TOP)
		{
			offset_y = -height;
		}
		else
		{
			offset_y = -height / (float)2.;
		}

		offset_x += XOFFS;
		offset_y += YOFFS;

		if (element->hasForeground)
		{
			D2->pBrush->SetColor(GISLibrary::ToD2D(PortrayalRender::Color(pc, element->foreground.token)));
			D2->pBrush->SetOpacity(1 - element->foreground.transparency);
		}

		for (auto itor = points.begin(); itor != points.end(); itor++)
		{
			GISLibrary::PointF* p = &(*itor);

			float x = p->x + offset_x;
			float y = p->y + offset_y;
			float _width = textMetrics->width;
			float _height = textMetrics->height;

			const wchar_t* vText = element->text.value.c_str();

			int vTextSize = (int)element->text.value.size();


			// rotate Point
			if (scaler->GetRotationDegree())
			{
				double _radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

				FLOAT tempX = (float)(scaler->soy - y) * (float)sin(_radian) + (float)(scaler->sox - x) * (float)cos(_radian);
				FLOAT tempY = (float)(scaler->soy - y) * (float)cos(_radian) - (float)(scaler->sox - x) * (float)sin(_radian);

				x = tempX + (float)scaler->sox;
				y = tempY + (float)scaler->soy;
			}

			D2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());
			D2->pRT->DrawText(
				vText,
				vTextSize,
				useWTF,
				D2D1::RectF(x, y, x + _width, y + _height),
				D2->pBrush
			);
		}
	}
}
