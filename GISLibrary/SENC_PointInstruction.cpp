#include "stdafx.h"
#include "SENC_PointInstruction.h"
#include "SENC_SpatialReference.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"
#include "SCommonFuction.h"
#include "SCurve.h"
#include "SMultiPoint.h"

#include "../GISLibrary/R_FeatureRecord.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../PortrayalCatalogue/S100_PointInstruction.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <ctime> 

SENC_PointInstruction::SENC_PointInstruction()
{
	type = 1;
}

SENC_PointInstruction::~SENC_PointInstruction()
{
	if (symbol)
	{
		delete symbol;
	}

	if (vectorPoint)
	{
		delete vectorPoint;
	}
}

void SENC_PointInstruction::GetDrawPoints(Scaler* scaler, std::vector<GISLibrary::PointF>& points)
{
	auto geom = fr->GetGeometry();	

	if (true == HasSpatialReference())
	{
		for (auto i = spatialReference.begin(); i != spatialReference.end(); i++)
		{
			auto sr = *i;
			// if Spatial Reference == Curve Record && Feature Type == Composite Curve
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
			// if Spatial Reference == Curve Record && Feature Type == Surface
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
	
	if (vectorPoint)
	{
		GISLibrary::PointF tempPoint;
		double mx = vectorPoint->GetX();
		double my = vectorPoint->GetY();

		projection(mx, my);
		scaler->WorldToDevice_F(mx, my, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (geom && geom->GetType() == SGeometryType::MultiPoint)
	{
		// A point symbol of a feature of many points (a default symbol over soundings the rules
		// do not know) is at each of them: the geometry has no one position of its own.
		auto multiPoint = static_cast<SMultiPoint*>(geom);
		for (int i = 0; i < multiPoint->GetNumPoints(); i++)
		{
			GISLibrary::PointF tempPoint;
			scaler->WorldToDevice_F(multiPoint->GetX(i), multiPoint->GetY(i), &tempPoint.x, &tempPoint.y);
			points.push_back(tempPoint);
		}
	}
	else if (geom)
	{
		GISLibrary::PointF tempPoint;

		auto mx = geom->GetX();
		auto my = geom->GetY();

		scaler->WorldToDevice_F(mx, my, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
}

void SENC_PointInstruction::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue */*pc*/, PCOutputSchemaManager* output)
{
	auto s100PointInstruction = (S100_PointInstruction*)s100Instruction;
	output->GetSENCFromS100Common(s100Instruction, this);

	if (s100PointInstruction->GetSymbol())
	{
		symbol = new SENC_Symbol();

		SENC_Symbol* s = symbol;

		s->reference = s100PointInstruction->GetSymbol()->GetReferenceW();

		if (s100PointInstruction->GetSymbol()->GetRotation())
		{
			s->rotation = s100PointInstruction->GetSymbol()->GetRotation();
		}
		s->rotationCRS = SENC_CommonFuc::GetRotationCRS(s100PointInstruction->GetSymbol()->GetRotationCRS());

		s->scaleFactor = s100PointInstruction->GetSymbol()->GetScaleFactor();

		if (s100PointInstruction->GetVectorPoint())
		{
			if (!vectorPoint)
			{
				vectorPoint = new SENC_VectorPoint();
			}
			vectorPoint->x = s100PointInstruction->GetVectorPoint()->GetX();
			vectorPoint->y = s100PointInstruction->GetVectorPoint()->GetY();
		}
	}
}