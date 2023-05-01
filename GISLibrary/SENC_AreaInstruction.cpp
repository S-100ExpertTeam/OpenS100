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
				D2D1_COLOR_F color = colorFill->color->RGBColor;
				//color.r = (float)(GetRValue(colorFill->color->RGBColor) / 255.0);
				//color.g = (float)(GetGValue(colorFill->color->RGBColor) / 255.0);
				//color.b = (float)(GetBValue(colorFill->color->RGBColor) / 255.0);
				
				color.a = 1 - colorFill->color->transparency;
				brush->SetColor(color);

				if (scaler->GetCurrentScale() >= SCommonFuction::NewGeometryScale)
				{
					auto geometry = surface->GetD2Geometry();
					if (geometry)
					{
						rt->FillGeometry(geometry, brush);
						//rt->DrawGeometry(geometry, brush, 1, strokeGroup->at(0));
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

							auto featureMBR = fr->GetMBR();
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

							auto featureMBR = fr->GetMBR();
							long sxmin = 0;
							long symin = 0;
							scaler->WorldToDevice(featureMBR.xmin, featureMBR.ymax, &sxmin, &symin);

							i->second->pBitmapBrush->SetTransform(D2D1::Matrix3x2F::Identity());
							i->second->pBitmapBrush->SetTransform(D2D1::Matrix3x2F::Rotation(-scaler->GetRotationDegree()));
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

			//if (s100SymbolFill->GetSymbol())
			//{
			//	symbolFill->symbol = new SENC_Symbol();
			//	symbolFill->symbol->reference = s100SymbolFill->GetSymbol()->GetReference();

			//	if (s100SymbolFill->GetSymbol()->GetRotation())
			//	{
			//		symbolFill->symbol->rotation = s100SymbolFill->GetSymbol()->GetRotation();
			//	}

			//	symbolFill->symbol->rotationCRS = SENC_CommonFuc::GetRotationCRS(s100SymbolFill->GetSymbol()->GetRotationCRS());
			//	symbolFill->symbol->scaleFactor = (float)(_wtof(s100SymbolFill->GetSymbol()->GetScaleFactor().c_str()));
			//}

			//if (s100SymbolFill->GetV1())
			//{
			//	symbolFill->v1.x = s100SymbolFill->GetV1()->GetX();
			//	symbolFill->v1.y = s100SymbolFill->GetV1()->GetY();
			//}
			//if (s100SymbolFill->GetV2())
			//{
			//	symbolFill->v2.x = s100SymbolFill->GetV2()->GetX();
			//	symbolFill->v2.y = s100SymbolFill->GetV2()->GetY();
			//}
		}
	}
}