#include "stdafx.h"
#include "D2DInstructionPainter.h"
#include "D2DConvert.h"
#include "D2DGeometryCache.h"
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
#include "../S100Engine/AreaFill.h"
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
	// The area fill the pattern bitmap is built from, for a symbol fill given in the instruction.
	std::unique_ptr<AreaFill> ToAreaFill(const SENC_SymbolFill& symbolFill)
	{
		auto areaFill = std::make_unique<AreaFill>();
		areaFill->v1_x = symbolFill.v1.x;
		areaFill->v1_y = symbolFill.v1.y;
		areaFill->v2_x = symbolFill.v2.x;
		areaFill->v2_y = symbolFill.v2.y;
		areaFill->_symbolReference = pugi::as_utf8(symbolFill.symbol->reference);
		areaFill->offset_x = symbolFill.symbol->offset.x;
		areaFill->offset_y = symbolFill.symbol->offset.y;
		return areaFill;
	}
}

namespace GISLibrary
{
	D2DInstructionPainter::D2DInstructionPainter(D2D1Resources* d2, Scaler* scaler, PortrayalCatalogue* pc)
		: d2(d2), scaler(scaler), pc(pc), renderer(d2, pc),
		shared(renderer, *scaler, [pc](const std::wstring& token) { return PortrayalRender::Color(pc, token); })
	{
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
					auto _areaFill = ToAreaFill(*symbolFill);
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

// ---- Line instructions: drawn by the shared painter, on Direct2D through D2DRenderer
void GISLibrary::D2DInstructionPainter::Visit(SENC_LineInstruction& instruction)
{
	PaintOnRenderer(instruction);
}

// ---- Augmented ray and path: drawn by the shared painter, on Direct2D through D2DRenderer
void GISLibrary::D2DInstructionPainter::Visit(SENC_AugmentedRay& instruction)
{
	PaintOnRenderer(instruction);
}

void GISLibrary::D2DInstructionPainter::Visit(SENC_AugmentedPath& instruction)
{
	PaintOnRenderer(instruction);
}

void GISLibrary::D2DInstructionPainter::PaintOnRenderer(SENC_Instruction& instruction)
{
	if (!renderer.Ready())
	{
		return;
	}
	instruction.Accept(shared);
}

// ---- Point instructions: drawn by the shared painter as well
void GISLibrary::D2DInstructionPainter::Visit(SENC_PointInstruction& instruction)
{
	PaintOnRenderer(instruction);
}

// ---- Text instructions: drawn by the shared painter as well
void GISLibrary::D2DInstructionPainter::Visit(SENC_TextInstruction& instruction)
{
	PaintOnRenderer(instruction);
}
