#include "stdafx.h"
#include "SENC_PointInstruction.h"
#include "SENC_SpatialReference.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"

#include "..\\GISLibrary\\R_FeatureRecord.h"

#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\S100Geometry\\SCurve.h"

#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"

#include "..\\PortrayalCatalogue\\S100_PointInstruction.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

#include <ctime> 
#include <mmsystem.h> 

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

#pragma warning(disable:4244)
void SENC_PointInstruction::GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points)
{
	if (nullptr == fr || nullptr == fr->m_geometry)
	{
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

	if (symbol)
	{
		rotation = symbol->rotation;
		scaleFactor = symbol->scaleFactor;
	}

	if (vectorPoint)
	{
		SPoint geo;
		geo.x = vectorPoint->x;
		geo.y = vectorPoint->y;
		projection(geo.x, geo.y);
		scaler->WorldToDevice_F(geo.x, geo.y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (HasSpatialReference() == true)
	{
		for (auto i = spatialReference.begin(); i != spatialReference.end(); i++)
		{
			auto sr = *i;
			
			// Point
			if (sr->RCNM == 110)
			{
				
			}
			// Multi point
			else if (sr->RCNM == 115)
			{
			}
			// Curve
			else if (sr->RCNM == 120 && 
				fr->m_geometry->GetType() == SGeometryType::CompositeCurve)
			{
				SCompositeCurve* geo = (SCompositeCurve*)fr->m_geometry;
				for (auto j = geo->m_listCurveLink.begin(); j != geo->m_listCurveLink.end(); j++)
				{
					//auto curve = j->GetCurve();
					auto curve = *j;
					auto rcid = curve->GetRCID();
					if (rcid == sr->reference)
					{
						int numPoints = curve->GetNumPoints();

						POINT *screenPoints = new POINT[numPoints];

						for (int k = 0; k < numPoints; k++)
						{
							scaler->WorldToDevice(
								curve->GetX(k), curve->GetY(k),
								&screenPoints[k].x, &screenPoints[k].y);
						}

						POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(screenPoints, numPoints, &scaler->GetScreenRect());

						if (symbolPoint)
						{
							D2D1_POINT_2F d2Point;
							d2Point.x = symbolPoint->x;
							d2Point.y = symbolPoint->y;
							delete symbolPoint;
							points.push_back(d2Point);
						}

						delete[] screenPoints;
						screenPoints = nullptr;
					}
				}
			}
			else if (sr->RCNM == 120 && 
				fr->m_geometry->GetType() == SGeometryType::CurveHasOrient)
			{
				SCurveHasOrient* curve = (SCurveHasOrient*)fr->m_geometry;
				
				auto rcid = curve->GetRCID();
				if (rcid == sr->reference)
				{
					int numPoints = curve->GetNumPoints();

					POINT* screenPoints = new POINT[numPoints];

					for (int k = 0; k < numPoints; k++)
					{
						scaler->WorldToDevice(
							curve->GetX(k), curve->GetY(k),
							&screenPoints[k].x, &screenPoints[k].y);
					}

					POINT* symbolPoint = SCommonFuction::GetCenterPointOfCurve(screenPoints, numPoints, &scaler->GetScreenRect());

					if (symbolPoint)
					{
						D2D1_POINT_2F d2Point;
						d2Point.x = symbolPoint->x;
						d2Point.y = symbolPoint->y;
						delete symbolPoint;
						points.push_back(d2Point);
					}

					delete[] screenPoints;
					screenPoints = nullptr;
				}
			}
			else if (sr->RCNM == 120 && 
				fr->m_geometry->GetType() == SGeometryType::Surface)
			{
				SSurface* geo = (SSurface*)fr->m_geometry;

				for (auto j = geo->curveList.begin(); j != geo->curveList.end(); j++)
				//for (auto j = geo->compositeCurve->m_listCurveLink.begin(); j != geo->compositeCurve->m_listCurveLink.end(); j++)
				{
					//auto curve = j->GetCurve();
					auto curve = (*j);
					auto rcid = curve->GetRCID();
					if (rcid == sr->reference)
					{
						int numPoints = curve->GetNumPoints();
						POINT *screenPoints = new POINT[numPoints];

						for (int k = 0; k < numPoints; k++)
						{
							scaler->WorldToDevice(
								curve->GetX(k), curve->GetY(k),
								&screenPoints[k].x, &screenPoints[k].y);
						}

						POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(screenPoints, numPoints, &scaler->GetScreenRect());

						if (symbolPoint)
						{
							D2D1_POINT_2F d2Point;
							d2Point.x = symbolPoint->x;
							d2Point.y = symbolPoint->y;
							delete symbolPoint;
							points.push_back(d2Point);
						}

						delete[] screenPoints;
						screenPoints = nullptr;
					}
				}
			}
			// Composite curve
			else if (sr->RCNM == 125)
			{
			}
			// Surface
			else if (sr->RCNM == 130)
			{
			}
		}
	}
	// When the feature is the point type,
	else if (fr->m_geometry->GetType() == SGeometryType::Point)
	{
		SPoint* geo = (SPoint*)fr->m_geometry;
		scaler->WorldToDevice_F(geo->x, geo->y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (fr->m_geometry->GetType() == SGeometryType::CompositeCurve)
	{
		SCompositeCurve* geo = (SCompositeCurve*)fr->m_geometry;
		for (auto lcl = geo->m_listCurveLink.begin(); lcl != geo->m_listCurveLink.end(); lcl++)
		{
			bDraw = false;
			//SCurve* c = (*lcl).GetCurve();
			SCurve* c = *lcl;
			
			if (!(*lcl)->GetMasking())
			{
				for (auto index = 0; index < c->GetNumPoints(); index++)
				{
					if (scaler->GetMapCalcMBR().PtInMBR(c->m_pPoints[index].x, c->m_pPoints[index].y))
					{
						if(viewPointNum == 0 && index > 1)
						{
							scaler->WorldToDevice(c->m_pPoints[index - 1].x, c->m_pPoints[index - 1].y,
								&SGeometry::viewPoints[viewPointNum].x, &SGeometry::viewPoints[viewPointNum].y);
							viewPointNum++;
						}
						scaler->WorldToDevice(c->m_pPoints[index].x, c->m_pPoints[index].y,
							&SGeometry::viewPoints[viewPointNum].x, &SGeometry::viewPoints[viewPointNum].y);
						viewPointNum++;
					}	
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
	else if (fr->m_geometry->GetType() == SGeometryType::Surface)
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
		}
	}
	else if (fr->m_geometry->GetType() == SGeometryType::Curve)
	{
		SCurve* c = (SCurve*)fr->m_geometry;

		for (auto index = 0; index < c->GetNumPoints(); index++)
		{
			if (scaler->GetMapCalcMBR().PtInMBR(c->m_pPoints[index].x, c->m_pPoints[index].y))
			{
				if (viewPointNum == 0 && index > 1)
				{
					scaler->WorldToDevice(c->m_pPoints[index - 1].x, c->m_pPoints[index - 1].y,
						&SGeometry::viewPoints[viewPointNum].x, &SGeometry::viewPoints[viewPointNum].y);
					viewPointNum++;
				}
				scaler->WorldToDevice(c->m_pPoints[index].x, c->m_pPoints[index].y,
					&SGeometry::viewPoints[viewPointNum].x, &SGeometry::viewPoints[viewPointNum].y);
				viewPointNum++;
			}
		}

		if (viewPointNum)
		{
			POINT* symbolPoint = SCommonFuction::GetCenterPointOfCurve(SGeometry::viewPoints, viewPointNum, &scaler->GetScreenRect());
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
}

#pragma warning(disable:4244)
void SENC_PointInstruction::GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points)
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

	if (symbol)
	{
		rotation = symbol->rotation;
		scaleFactor = symbol->scaleFactor;
	}


	DWORD dwWorkStart = timeGetTime();
	if (vectorPoint)
	{
		SPoint geo;
		geo.x = vectorPoint->x;
		geo.y = vectorPoint->y;
		projection(geo.x, geo.y);
		scaler->WorldToDevice_F(geo.x, geo.y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (true == HasSpatialReference())
	{
		for (auto i = spatialReference.begin(); i != spatialReference.end(); i++)
		{
			auto sr = *i;
			if (sr->RCNM == 110)
			{
			}
			else if (sr->RCNM == 115)
			{
			}
			else if (sr->RCNM == 120 && 
				fr->m_geometry->GetType() == SGeometryType::CompositeCurve)
			{
				SCompositeCurve* geo = (SCompositeCurve*)fr->m_geometry;
				for (auto j = geo->m_listCurveLink.begin(); j != geo->m_listCurveLink.end(); j++)
				{
					//auto curve = j->GetCurve();
					auto curve = *j;
					auto rcid = curve->GetRCID();
					if (rcid == sr->reference)
					{
						int numPoints = curve->GetNumPoints();

						POINT *screenPoints = new POINT[numPoints];

						for (int k = 0; k < numPoints; k++)
						{
							scaler->WorldToDevice(
								curve->GetX(k), curve->GetY(k),
								&screenPoints[k].x, &screenPoints[k].y);
						}

						POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(screenPoints, numPoints, &scaler->GetScreenRect());

						if (symbolPoint)
						{
							D2D1_POINT_2F d2Point;
							d2Point.x = symbolPoint->x;
							d2Point.y = symbolPoint->y;
							delete symbolPoint;
							points.push_back(d2Point);
						}

						delete[] screenPoints;
						screenPoints = nullptr;
					}
				}
			}
			else if (sr->RCNM == 120 && 
				fr->m_geometry->GetType() == SGeometryType::Surface)
			{
				SSurface* geo = (SSurface*)fr->m_geometry;

				for (auto j = geo->curveList.begin(); j != geo->curveList.end(); j++)
				//for (auto j = geo->compositeCurve->m_listCurveLink.begin(); j != geo->compositeCurve->m_listCurveLink.end(); j++)
				{
					//auto curve = j->GetCurve();
					auto curve = (*j);
					auto rcid = curve->GetRCID();
					if (rcid == sr->reference)
					{
						int numPoints = curve->GetNumPoints();
						POINT *screenPoints = new POINT[numPoints];

						for (int k = 0; k < numPoints; k++)
						{
							scaler->WorldToDevice(
								curve->GetX(k), curve->GetY(k),
								&screenPoints[k].x, &screenPoints[k].y);
						}

						POINT *symbolPoint = SCommonFuction::GetCenterPointOfCurve(screenPoints, numPoints, &scaler->GetScreenRect());

						if (symbolPoint)
						{
							D2D1_POINT_2F d2Point;
							d2Point.x = symbolPoint->x;
							d2Point.y = symbolPoint->y;
							delete symbolPoint;
							points.push_back(d2Point);
						}

						delete[] screenPoints;
						screenPoints = nullptr;
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
	else if (fr->m_geometry->GetType() == SGeometryType::Point)
	{
		SPoint* geo = (SPoint*)fr->m_geometry;
		scaler->WorldToDevice_F(geo->x, geo->y, &tempPoint.x, &tempPoint.y);
		points.push_back(tempPoint);
	}
	else if (fr->m_geometry->GetType() == SGeometryType::CompositeCurve)
	{
		SCompositeCurve* geo = (SCompositeCurve*)fr->m_geometry;

		for (auto lcl = geo->m_listCurveLink.begin(); lcl != geo->m_listCurveLink.end(); lcl++)
		{
			bDraw = false;
			//SCurve* c = (*lcl).GetCurve();
			SCurve* c = (*lcl);
			
			if (!(*lcl)->GetMasking())
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
	else if (fr->m_geometry->GetType() == SGeometryType::Surface)
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
	}
}

void SENC_PointInstruction::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100PointInstruction = (S100_PointInstruction*)s100Instruction;
	output->GetSENCFromS100Common(s100Instruction, this);

	if (s100PointInstruction->GetSymbol())
	{
		symbol = new SENC_Symbol();

		SENC_Symbol* s = symbol;

		s->reference = s100PointInstruction->GetSymbol()->GetReference();

		if (s100PointInstruction->GetSymbol()->GetRotation())
		{
			s->rotation = s100PointInstruction->GetSymbol()->GetRotation();
		}
		s->rotationCRS = SENC_CommonFuc::GetRotationCRS(s100PointInstruction->GetSymbol()->GetRotationCRS());

		if (s100PointInstruction->GetSymbol()->GetScaleFactor().size() == 0)
		{
			s->scaleFactor = 1.0;
		}
		else
		{
			s->scaleFactor = (float)(_wtof(s100PointInstruction->GetSymbol()->GetScaleFactor().c_str()));
		}

		if (s100PointInstruction->GetSymbol()->GetLinePlacement())
		{
			s->linePlacement = new SENC_LinePlacement();
			s->linePlacement->offset = (float)(_wtof(s100PointInstruction->GetSymbol()->GetLinePlacement()->GetOffset().c_str()));
			s->linePlacement->placementMode = SENC_CommonFuc::GetLinePlacement(s100PointInstruction->GetSymbol()->GetLinePlacement()->GetPlacementMode());
		}
		if (s100PointInstruction->GetSymbol()->GetAreaPlacement())
		{
			s->areaPlacement = new SENC_AreaPlacement();
			s->areaPlacement->placementMode = SENC_CommonFuc::GetAreaPlacement(s100PointInstruction->GetSymbol()->GetAreaPlacement()->GetPlacementMode());
		}

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