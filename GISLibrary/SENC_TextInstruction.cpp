#include "stdafx.h"
#include "DebugOutput.h"
#include "SENC_TextInstruction.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"
#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SCommonFuction.h"
#include "SENC_SpatialReference.h"

#include "../GISLibrary/R_FeatureRecord.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_TextInstruction.h"

#include <ctime> 

SENC_TextInstruction::SENC_TextInstruction()
{
	type = 5;
	textPoint = NULL;
}

SENC_TextInstruction::~SENC_TextInstruction()
{
	if (textPoint) delete textPoint;
}

void rotate_v(double &nx, double &ny, double tx, double ty, double cx, double cy, double rad)
{
	double cosRad = cos(rad);
	double sinRad = sin(rad);

	tx -= cx;
	ty -= cy;

	nx = tx * cosRad - ty * sinRad;
	ny = ty * cosRad + tx * sinRad;

	nx += cx, ny += cy;
}

//
void SENC_TextInstruction::GetDrawPoints(Scaler *scaler, std::vector<GISLibrary::PointF> &points)
{
	auto geom = fr->GetGeometry();

	if (true == HasSpatialReference())
	{
		for (auto i = spatialReference.begin(); i != spatialReference.end(); i++)
		{
			auto sr = *i;
			if (sr->RCNM == 120 && geom->GetType() == SGeometryType::CompositeCurve)
			{
				SCompositeCurve* geo = (SCompositeCurve*)geom;
				std::list<SCurve*> curveList;
				geo->GetCurveList(curveList);
				for (auto j = curveList.begin(); j != curveList.end(); j++)
				{
					auto curve = *j;
					auto rcid = curve->GetRCID();
					if (rcid == sr->reference)
					{
						double mx = curve->getCenterPoint().GetX();
						double my = curve->getCenterPoint().GetY();
						GISLibrary::PointF sp;
						scaler->WorldToDevice_F(mx, my, &sp.x, &sp.y);
						points.push_back(sp);
					}
				}
			}
			else if (sr->RCNM == 120 && geom->GetType() == SGeometryType::Surface)
			{
				SSurface* geo = (SSurface*)geom;
				std::list<SCurve*> curveList;
				geo->GetCurveList(curveList);

				for (auto j = curveList.begin(); j != curveList.end(); j++)
				{
					auto curve = (*j);
					auto rcid = curve->GetRCID();
					if (rcid == sr->reference)
					{
						double mx = curve->getCenterPoint().GetX();
						double my = curve->getCenterPoint().GetY();
						GISLibrary::PointF sp;
						scaler->WorldToDevice_F(mx, my, &sp.x, &sp.y);
						points.push_back(sp);
					}
				}
			}
			else if (sr->RCNM == 125)
			{
			}
			else if (sr->RCNM == 130)
			{
			}
		}
	}
	else
	{
		GISLibrary::PointF tempPoint;

		auto mx = geom->GetX();
		auto my = geom->GetY();

		scaler->WorldToDevice_F(mx, my, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
}

void SENC_TextInstruction::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue* /*pc*/, PCOutputSchemaManager* output)
{
	auto s100TextInstruction = (S100_TextInstruction*)s100Instruction;

	output->GetSENCFromS100Common(s100Instruction, this);
	if (s100TextInstruction->GetTextPoint())
	{
		textPoint = new SENC_TextPoint();

		textPoint->horizontalAlignment = SENC_CommonFuc::GetHorizontalAlignment(s100TextInstruction->GetTextPoint()->GetHorizontalAlignmentW());
		textPoint->verticalAlignment = SENC_CommonFuc::GetVerticalAlignment(s100TextInstruction->GetTextPoint()->GetVerticalAlignmentW());
		if (s100TextInstruction->GetTextPoint()->GetOffset())
		{
			textPoint->offset.x = s100TextInstruction->GetTextPoint()->GetOffset()->GetX();
			textPoint->offset.y = s100TextInstruction->GetTextPoint()->GetOffset()->GetY();
		}

		auto elements = s100TextInstruction->GetTextPoint()->GetElemets();
		for (auto itor = elements.begin(); itor != elements.end(); itor++)
		{
			S100_Element* e1 = *itor;
			SENC_Element* e2 = new SENC_Element();

			textPoint->elements.push_back(e2);

			e2->bodySize = static_cast<int>(std::wcstol(e1->GetBodySizeW().c_str(), nullptr, 10));
			e2->verticalOffset = (float)(std::wcstod(e1->GetVerticalOffsetW().c_str(), nullptr));

			if (e1->GetFont())
			{
				e2->font.proportion = SENC_CommonFuc::GetFontProportion(e1->GetFont()->GetProportionW());
				e2->font.serifs = static_cast<int>(std::wcstol(e1->GetFont()->GetSerifsW().c_str(), nullptr, 10)) != 0 ? true : false;

				if (true == e1->GetFont()->isUpright())
				{
					e2->font.setUpright();
				}
				else
				{
					e2->font.setItalics();
				}

				//e2->font.slant = static_cast<int>(std::wcstol(e1->GetFont()->GetSlant().c_str(), nullptr, 10));
				e2->font.weight = static_cast<int>(std::wcstol(e1->GetFont()->GetWeightW().c_str(), nullptr, 10));
			}

			if (e1->GetForground())
			{
				e2->foreground.token = e1->GetForground()->GetTokenW();
				e2->foreground.transparency = (int)(std::wcstod(e1->GetForground()->GetTransparencyW().c_str(), nullptr) * 100);
				e2->hasForeground = true;
			}

			if (e1->GetText())
			{
				e2->text.value = e1->GetText()->GetValueW();
				e2->text.useValueOf = e1->GetText()->GetUseValueOfW();
			}
		}
	}
}
