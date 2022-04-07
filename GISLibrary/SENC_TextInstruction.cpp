#include "stdafx.h"
#include "SENC_TextInstruction.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"

#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\S100Geometry\\SCurve.h"
#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"
#include "..\\PortrayalCatalogue\\S100_TextInstruction.h"

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


DWORD dwTimeCheck_TextInstruction[100] = { 0 };
int iDebugCount_TextInstruction[100] = { 0 };

#pragma warning(disable:4244)
void SENC_TextInstruction::GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points)
{
	if (!fr)
		return;
	if (!fr->m_geometry)
		return;

	double rotation = 0;
	double scaleFactor = 1;
	int viewPointNum = 0;
	int partsIndex = 1;
	int lastPointIndex = -1;
	unsigned preLoc = -1, curLoc = 0;
	D2D1_POINT_2F tempPoint;

	bool bDraw;

	DWORD dwWorkStart = timeGetTime();

	if (fr->m_geometry->type == 1)
	{
		SPoint* geo = (SPoint*)fr->m_geometry;
		scaler->WorldToDevice_F(geo->m_point.x, geo->m_point.y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
		dwTimeCheck_TextInstruction[1] += timeGetTime() - dwWorkStart;
		iDebugCount_TextInstruction[1]++;
	}
	else if (fr->m_geometry->type == 2)
	{
		SCompositeCurve* geo = (SCompositeCurve*)fr->m_geometry;

		for (auto lcl = geo->m_listCurveLink.begin(); lcl != geo->m_listCurveLink.end(); lcl++)
		{
			bDraw = false;
			SCurve* c = (*lcl).GetCurve();

			if (!(*lcl).GetMasking())
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
		dwTimeCheck_TextInstruction[2] += timeGetTime() - dwWorkStart;
		iDebugCount_TextInstruction[2]++;
	}
	else if (fr->m_geometry->type == 3)
	{
		SSurface* geo = (SSurface*)fr->m_geometry;

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

		dwTimeCheck_TextInstruction[3] += timeGetTime() - dwWorkStart;
		iDebugCount_TextInstruction[3]++;
	}


}

void SENC_TextInstruction::GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points)
{
	if (!fr)
		return;
	if (!fr->m_geometry)
		return;

	double rotation = 0;
	double scaleFactor = 1;
	int viewPointNum = 0;
	int partsIndex = 1;
	int lastPointIndex = -1;
	unsigned preLoc = -1, curLoc = 0;
	D2D1_POINT_2F tempPoint;

	bool bDraw;

	DWORD dwWorkStart = timeGetTime();

	if (fr->m_geometry->type == 1)
	{
		SPoint* geo = (SPoint*)fr->m_geometry;
		scaler->WorldToDevice_F(geo->m_point.x, geo->m_point.y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
		dwTimeCheck_TextInstruction[1] += timeGetTime() - dwWorkStart;
		iDebugCount_TextInstruction[1]++;
	}
	else if (fr->m_geometry->type == 2)
	{
		SCompositeCurve* geo = (SCompositeCurve*)fr->m_geometry;

		for (auto lcl = geo->m_listCurveLink.begin(); lcl != geo->m_listCurveLink.end(); lcl++)
		{
			bDraw = false;
			SCurve* c = (*lcl).GetCurve();

			if (!(*lcl).GetMasking())
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
		dwTimeCheck_TextInstruction[2] += timeGetTime() - dwWorkStart;
		iDebugCount_TextInstruction[2]++;
	}
	else if (fr->m_geometry->type == 3)
	{
		SSurface* geo = (SSurface*)fr->m_geometry;

		if (geo->m_centerPoint)
		{
			scaler->WorldToDevice_F(geo->m_centerPoint->x, geo->m_centerPoint->y, &tempPoint.x, &tempPoint.y);
			points.push_back(tempPoint);
		}
		else
		{
		}

		dwTimeCheck_TextInstruction[3] += timeGetTime() - dwWorkStart;
		iDebugCount_TextInstruction[3]++;
	}
}

void SENC_TextInstruction::ChangePallete(PortrayalCatalogue* pc)
{
	if (textPoint)
	{
		auto colorProfile = pc->GetColorProfile();
		if (colorProfile)
		{
			auto itorTp = textPoint->elements.begin();

			if (itorTp != textPoint->elements.end())
			{
				SENC_Element* element = *itorTp;

				if (element->foreground.token.size() > 0)
				{
					element->pColor = new COLORREF();
					*element->pColor = colorProfile->GetRGBRef(pc->GetCurrentPaletteName(), element->foreground.token);
				}
				else
				{
					element->pColor = NULL;
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
				e2->font.slant = _wtoi(e1->GetFont()->GetSlant().c_str());
				e2->font.weight = _wtoi(e1->GetFont()->GetWeight().c_str());
			}

			if (e1->GetForground())
			{
				e2->foreground.token = e1->GetForground()->GetToken();
				e2->foreground.transparency = (int)(_wtof(e1->GetForground()->GetTransparency().c_str()) * 100);
			}
			if (e1->GetText())
			{
				e2->text.value = e1->GetText()->GetValue();
				e2->text.useValueOf = e1->GetText()->GetUseValueOf();
			}
		}
	}
}