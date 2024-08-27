#include "stdafx.h"
#include "SENC_TextInstruction.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"
#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SCommonFuction.h"

#include "../GISLibrary/R_FeatureRecord.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_TextInstruction.h"

#include <ctime> 
#include <mmsystem.h> 

SENC_TextInstruction::SENC_TextInstruction()
{
	type = 5;
	textPoint = NULL;
}

SENC_TextInstruction::~SENC_TextInstruction()
{
	if (textPoint) delete textPoint;
	if (textMatrix) delete textMatrix;
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

#pragma warning(disable:4244)
void SENC_TextInstruction::GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points)
{
	if (!fr || !fr->GetGeometry()) {
		return;
	}

	auto geom = fr->GetGeometry();
		
	int viewPointNum = 0;
	D2D1_POINT_2F tempPoint;

	bool bDraw;

	DWORD dwWorkStart = timeGetTime();

	if (geom->GetType() == SGeometryType::Point)
	{
		SPoint* geo = (SPoint*)geom;
		scaler->WorldToDevice_F(geo->x, geo->y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (geom->GetType() == SGeometryType::CompositeCurve)
	{
		SCompositeCurve* geo = (SCompositeCurve*)geom;
		std::list<SCurve*> curveList;
		geo->GetCurveList(curveList);

		for (auto i = curveList.begin(); i != curveList.end(); i++)
		{
			bDraw = false;
			auto c = (*i);

			if (!c->GetMasking())
			{
				for (auto index = 0; index < c->GetNumPoints(); index++)
				{
					scaler->WorldToDevice(c->m_pPoints[index].x, c->m_pPoints[index].y,
						&SGeometry::viewPoints[viewPointNum].x, &SGeometry::viewPoints[viewPointNum].y);

					viewPointNum++;
				}
			}
			else
			{
				bDraw = true;
			}

			if (viewPointNum || bDraw)
			{
				POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(SGeometry::viewPoints, viewPointNum, &scaler->GetScreenRect());
				if (symbolPoint)
				{
					tempPoint.x = (float)symbolPoint[0].x;
					tempPoint.y = (float)symbolPoint[0].y;
					points.push_back(tempPoint);

					delete symbolPoint;
				}
				viewPointNum = 0;
			}
		}

		if (viewPointNum)
		{
			POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(SGeometry::viewPoints, viewPointNum, &scaler->GetScreenRect());
			if (symbolPoint)
			{
				tempPoint.x = (float)symbolPoint[0].x;
				tempPoint.y = (float)symbolPoint[0].y;
				points.push_back(tempPoint);

				delete symbolPoint;
			}
			viewPointNum = 0;
		}
	}
	else if (geom->GetType() == SGeometryType::Surface)
	{
		SSurface* geo = (SSurface*)geom;

		// need coordinates that are not clipped.
		std::vector<POINT> vp;
		SCommonFuction::CalculateCenterOfGravityOfSurface(vp, geo, &CRect(scaler->sxMin, scaler->syMin, scaler->sxMax, scaler->syMax), scaler);

		for (auto itor = vp.begin(); itor != vp.end(); itor++)
		{
			tempPoint.x = (float)(*itor).x;
			tempPoint.y = (float)(*itor).y;
			points.push_back(tempPoint);

			break;
		}
	}
}

void SENC_TextInstruction::GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points)
{
	if (!fr) {
		return;
	}

	auto geom = fr->GetGeometry();

	if (!geom) {
		return;
	}

	double rotation = 0;
	double scaleFactor = 1;
	int viewPointNum = 0;
	int partsIndex = 1;
	int lastPointIndex = -1;
	unsigned preLoc = -1, curLoc = 0;
	D2D1_POINT_2F tempPoint;

	bool bDraw;

	DWORD dwWorkStart = timeGetTime();

	if (geom->GetType() == SGeometryType::Point)
	{
		SPoint* geo = (SPoint*)geom;
		scaler->WorldToDevice_F(geo->x, geo->y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (geom->GetType() == SGeometryType::CompositeCurve)
	{
		SCompositeCurve* geo = (SCompositeCurve*)geom;
		std::list<SCurve*> curveList;
		geo->GetCurveList(curveList);

		for (auto i = curveList.begin(); i != curveList.end(); i++)
		{
			bDraw = false;
			auto c = (*i);

			if (!c->GetMasking())
			{
				for (auto index = 0; index < c->GetNumPoints(); index++)
				{
					scaler->WorldToDevice(c->m_pPoints[index].x, c->m_pPoints[index].y,
						&SGeometry::viewPoints[viewPointNum].x, &SGeometry::viewPoints[viewPointNum].y);

					viewPointNum++;
				}
			}
			else
			{
				bDraw = true;
			}

			if (viewPointNum || bDraw)
			{
				POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(SGeometry::viewPoints, viewPointNum, &scaler->GetScreenRect());
				if (symbolPoint)
				{
					tempPoint.x = (float)symbolPoint[0].x;
					tempPoint.y = (float)symbolPoint[0].y;
					points.push_back(tempPoint);

					delete symbolPoint;
				}
				viewPointNum = 0;
			}
		}

		if (viewPointNum)
		{
			POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(SGeometry::viewPoints, viewPointNum, &scaler->GetScreenRect());
			if (symbolPoint)
			{
				tempPoint.x = (float)symbolPoint[0].x;
				tempPoint.y = (float)symbolPoint[0].y;
				points.push_back(tempPoint);

				delete symbolPoint;
			}
			viewPointNum = 0;
		}
	}
	else if (geom->GetType() == SGeometryType::Surface)
	{
		SSurface* geo = (SSurface*)geom;

		if (geo->m_centerPoint)
		{
			scaler->WorldToDevice_F(geo->m_centerPoint->x, geo->m_centerPoint->y, &tempPoint.x, &tempPoint.y);
			points.push_back(tempPoint);
		}
	}
}

void SENC_TextInstruction::ChangePallete(PortrayalCatalogue* pc)
{
	if (textPoint)
	{
		auto colorProfile = pc->GetS100PCManager()->GetS100ColorProfile();
		if (colorProfile)
		{
			auto itorTp = textPoint->elements.begin();

			if (itorTp != textPoint->elements.end())
			{
				SENC_Element* element = *itorTp;

				if (element->foreground.token.size() > 0)
				{
					element->pColor = new D2D1_COLOR_F();
					*element->pColor = colorProfile->GetColor(element->foreground.token);
				}
				else
				{
					element->pColor = nullptr;
				}
			}
		}
	}
}

void SENC_TextInstruction::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100TextInstruction = (S100_TextInstruction*)s100Instruction;

	output->GetSENCFromS100Common(s100Instruction, this);
	if (s100TextInstruction->GetTextPoint())
	{
		textPoint = new SENC_TextPoint();

		textPoint->horizontalAlignment = SENC_CommonFuc::GetHorizontalAlignment(s100TextInstruction->GetTextPoint()->GetHorizontalAlignment());
		textPoint->verticalAlignment = SENC_CommonFuc::GetVerticalAlignment(s100TextInstruction->GetTextPoint()->GetVerticalAlignment());
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

			e2->bodySize = _wtoi(e1->GetBodySize().c_str());
			e2->verticalOffset = (float)(_wtof(e1->GetVerticalOffset().c_str()));

			if (e1->GetFont())
			{
				e2->font.proportion = SENC_CommonFuc::GetFontProportion(e1->GetFont()->GetProportion());
				e2->font.serifs = _wtoi(e1->GetFont()->GetSerifs().c_str()) != 0 ? true : false;

				if (true == e1->GetFont()->isUpright())
				{
					e2->font.setUpright();
				}
				else
				{
					e2->font.setItalics();
				}

				//e2->font.slant = _wtoi(e1->GetFont()->GetSlant().c_str());
				e2->font.weight = _wtoi(e1->GetFont()->GetWeight().c_str());
			}

			if (e1->GetForground())
			{
				e2->foreground.token = e1->GetForground()->GetToken();
				e2->foreground.transparency = (int)(_wtof(e1->GetForground()->GetTransparency().c_str()) * 100);
				
				auto colorProfile = pc->GetS100PCManager()->GetS100ColorProfile();

				if (colorProfile)
				{
					auto color = colorProfile->GetColor(e2->foreground.token);
					e2->pColor = new D2D1_COLOR_F();
					*e2->pColor = color;
				}
			}

			if (e1->GetText())
			{
				e2->text.value = e1->GetText()->GetValue();
				e2->text.useValueOf = e1->GetText()->GetUseValueOf();
			}
		}
	}
}