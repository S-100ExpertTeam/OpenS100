#include "stdafx.h"
#include "SENC_AreaInstruction.h"
#include "SENC_ColorFill.h"
#include "SENC_AreaFillReference.h"
#include "SENC_PixmapFill.h"
#include "SENC_SymbolFill.h"
#include "SENC_HatchFill.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"
#include "GISLibrary.h"

#include "..\\S100Engine\\AreaPatternBitmap.h"

#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"
#include "..\\PortrayalCatalogue\\S100_ColorFill.h"
#include "..\\PortrayalCatalogue\\S100_AreaInstruction.h"
#include "..\\PortrayalCatalogue\\S100_AreaFillReference.h"

#include "..\\GISLibrary\\R_FeatureRecord.h"

#include "SCommonFuction.h"
#include "SCoverage.h"

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
void SENC_AreaInstruction::DrawInstruction(D2D1Resources* d2, Scaler *scaler, PortrayalCatalogue* pc)
{
	auto rt = d2->RenderTarget();
	auto pDirect2dFactory = d2->Factory();
	auto brush = d2->SolidColorBrush();
	if (!brush || !pDirect2dFactory || !rt)
	{
		return;
	}

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
				D2D1_COLOR_F color = colorFill->color->RGBColor;
				
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

				if (i == patternMap->end())
				{
					i = patternMap->find(L"QUESMRK1");
				}

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
				auto patternMap = &pc->GetS100PCManager()->areaFillInfo.patternMap;
				auto i = patternMap->find(symbolFill->GetFileTitle().c_str());

				ID2D1Brush* imageBrush = nullptr;
				AreaPatternBitmap* bitmapImage = nullptr;

				// Symbol Fill을 별도의 파일(AreaFills)에서 읽은 경우
				if (symbolFill->GetFileTitle().empty() == false)
				{
					if (i != patternMap->end())
					{
						imageBrush = i->second->pBitmapBrush;
					}
				}
				else
				{
					auto areaFill = symbolFill->as_AreaFill();
					bitmapImage = pc->GetS100PCManager()->CreateBitmapImage(areaFill.get(), pDirect2dFactory, d2->pImagingFactory, d2->SolidStrokeStyle());
					if (bitmapImage)
					{
						bitmapImage->pBitmapBrush = pc->GetS100PCManager()->CreateBitmapBrush(bitmapImage, rt);
						imageBrush = bitmapImage->pBitmapBrush;
						imageBrush->SetTransform(scaler->GetInverseMatrix());
					}
				}

				if (imageBrush != nullptr)
				{
					if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
					{
						auto geometry = surface->GetD2Geometry();
						if (geometry)
						{
							rt->FillGeometry(geometry, imageBrush);
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

				auto colorProfile = pc->GetS100PCManager()->GetS100ColorProfile();

				if (colorProfile)
				{
					colorFill->color->RGBColor = colorProfile->GetColor(colorFill->color->token);
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
			auto s100SymbolFill = (S100_SymbolFill*)s100AreaInstruction->GetAreaFill();

			symbolFill->areaCRSType = SENC_CommonFuc::GetAreaCRSType(s100SymbolFill->GetAreaCRS());

			areaFill->SetFileTitle(s100SymbolFill->GetFileTitle());

			auto s100Symbol = s100SymbolFill->GetSymbol();
			if (s100Symbol)
			{
				symbolFill->symbol = new SENC_Symbol();
				symbolFill->symbol->reference = s100Symbol->GetReference();
				if (s100Symbol->GetRotation())
				{
					symbolFill->symbol->rotation = s100Symbol->GetRotation();
				}
				symbolFill->symbol->rotationCRS = SENC_CommonFuc::GetRotationCRS(s100Symbol->GetRotationCRS());
				symbolFill->symbol->scaleFactor = s100Symbol->GetScaleFactor();


				auto svgSymbolManger = &(pc->GetS100PCManager()->s100SymbolManager);
				auto svg = svgSymbolManger->GetSVG(symbolFill->symbol->reference);

				if (svg == nullptr)
				{
					svg = svgSymbolManger->GetSVG(L"QUESMRK1");
				}
				else
				{
					symbolFill->symbol->pSvg = svg;
				}

				symbolFill->symbol->offset.x = s100Symbol->GetOffsetX();
				symbolFill->symbol->offset.y = s100Symbol->GetOffsetY();
			}

			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1())
			{
				symbolFill->v1.x = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1()->GetX();
				symbolFill->v1.y = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1()->GetY();
			}

			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2())
			{
				symbolFill->v2.x = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2()->GetX();
				symbolFill->v2.y = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2()->GetY();
			}
		}
	}
}