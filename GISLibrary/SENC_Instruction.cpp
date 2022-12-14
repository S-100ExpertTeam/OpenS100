#include "stdafx.h"
#include "SENC_Instruction.h"
#include "SENC_SymbolFill.h"
#include "SENC_LineStyle.h"
#include "SENC_ArcByRadius.h"
#include "SENC_SpatialReference.h"
#include "SENC_NullInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_AreaInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AugmentedRay.h"
#include "SENC_AugmentedPath.h"
#include "R_FeatureRecord.h"
#include "S100SpatialObject.h"

#include "..\\PortrayalCatalogue\\S100_Symbol.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"
#include "..\\PortrayalCatalogue\\S100_NullInstruction.h"
#include "..\\GeoMetryLibrary\\GeoPoint.h"
#include "..\\S100Geometry\\SSurface.h"
#include "..\\LatLonUtility\\LatLonUtility.h"

SENC_Instruction::SENC_Instruction()
{
	featureReference = 0;

	type = 0;
	suppressedInstance = false;
}

int SENC_Instruction::FeatureReference()
{
	return featureReference;
}

int SENC_Instruction::DrawingPriority()
{
	return drawingPriority;
}

SENC_Instruction::~SENC_Instruction()
{
	for (auto itor = spatialReference.begin(); itor != spatialReference.end(); itor++)
	{
		delete *itor;
	}
}

bool SENC_Instruction::HasSpatialReference()
{
	return spatialReference.size() > 0;
}

bool SENC_Instruction::IsEqualPrimitive()
{
	auto SPAS_RCNM = fr->GetSPASRCNM();

	auto prim = GeometryPrimitive();
	if (prim == 0)
	{
		if (SPAS_RCNM == 0 || SPAS_RCNM == -1)
		{
			return false;
		}
	}
	else if (prim == 1)
	{
		if (SPAS_RCNM == 110 || SPAS_RCNM == 115)
		{
			return true;
		}
	}
	else if (prim == 2)
	{

	}
	else if (prim == 3)
	{

	}

	return false;
}

int SENC_Instruction::GeometryPrimitive()
{
	if (type == 1 || type == 5 || type == 6 || type == 7 || type == 8)
	{
		return 1;
	}
	else if (type == 2)
	{
		return 2;
	}
	else if (type == 3 || type == 4 || type == 9)
	{
		return 3;
	}

	return 0;
}

void SENC_Instruction::CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler)
{
	std::vector<POINT> m_pvPoints;

	if (_surface->m_numPoints > SGeometry::sizeOfPoint)
	{
		SGeometry::sizeOfPoint = _surface->m_numPoints;

		delete[] SGeometry::viewPoints; 
		SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
	}

	for (int count = 0; count < _surface->m_numPoints; count++)
	{
		pScaler->WorldToDevice(_surface->m_pPoints[count].x, _surface->m_pPoints[count].y, &SGeometry::viewPoints[count].x, &SGeometry::viewPoints[count].y);
	}

	ClipperLib::Paths clipedPolygon = ClipSurface(_surface, _viewPort); // result of clipping the polygon that came as a factor,

	if (clipedPolygon.size() == 0)
	{
		return;
	}

	for (auto itor = clipedPolygon.begin(); itor != clipedPolygon.end(); itor++)
	{
		vp.push_back(CalculateCenterOfGravityOfSurface(*itor));
		break;
	}
	return;
}

ClipperLib::Paths SENC_Instruction::ClipSurface(SSurface *_surface, CRect *_viewPort)
{
	ClipperLib::Path  polygon, view;
	ClipperLib::Paths result;
	ClipperLib::Clipper  clipper;

	ClipperLib::IntPoint tmp;

	tmp.X = _viewPort->left;   // Specify the coordinates of the View port
	tmp.Y = _viewPort->top;    
	view.push_back(tmp);	   
	tmp.X = _viewPort->right;  
	tmp.Y = _viewPort->top;	   
	view.push_back(tmp);	   
	tmp.X = _viewPort->right;  
	tmp.Y = _viewPort->bottom; 
	view.push_back(tmp);	   
	tmp.X = _viewPort->left;   
	tmp.Y = _viewPort->bottom; 
	view.push_back(tmp);	   

	int part1PointCount;											// Find the number of points for the first part of the polygon.
	if (_surface->m_numParts > 1) 									
	{																
		part1PointCount = _surface->m_pParts[1] - _surface->m_pParts[0];	
	}																
	else 															
	{																
		part1PointCount = _surface->m_numPoints;						
	}																

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = _surface->SGeometry::viewPoints[i].x;
		tmp.Y = _surface->SGeometry::viewPoints[i].y;
		polygon.push_back(tmp);
	}

	clipper.AddPath(polygon, ClipperLib::ptSubject, true);
	clipper.AddPath(view, ClipperLib::ptClip, true);
	clipper.Execute(ClipperLib::ctIntersection, result, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	return result;
}


POINT SENC_Instruction::CalculateCenterOfGravityOfSurface(POINT *_p, int _count)
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	double x1, y1, x2, y2, tmpArea;

	for (int i = 0; i < _count; i++)
	{
		j = (i + 1) % _count;

		x1 = _p[i].x;
		y1 = _p[i].y;
		x2 = _p[j].x;
		y2 = _p[j].y;

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	POINT returnValue;
	returnValue.x = (LONG)centerX;
	returnValue.y = (LONG)centerY;

	return returnValue;
}


POINT SENC_Instruction::CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon)
{
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	ClipperLib::long64 x1, y1, x2, y2, tmpArea;

	int _count = (int)polygon.size();

	for (auto itor = polygon.begin(); itor != polygon.end(); itor++)
	{
		x1 = (*itor).X;
		y1 = (*itor).Y;

		if (itor == polygon.end() - 1)
		{
			x2 = (*polygon.begin()).X;
			y2 = (*polygon.begin()).Y;
		}
		else
		{
			x2 = (*(itor + 1)).X;
			y2 = (*(itor + 1)).Y;
		}

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	POINT returnValue;
	returnValue.x = (LONG)centerX;
	returnValue.y = (LONG)centerY;

	return returnValue;
}

double SENC_Instruction::GetDistance(POINT _p1, POINT _p2)
{
	GeoPoint p;

	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	return sqrt((p.x * p.x) + (p.y * p.y));
}

double SENC_Instruction::GetAngle(POINT _p1, POINT _p2)
{
	GeoPoint p;
	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	double d = sqrt((p.x * p.x) + (p.y * p.y));
	double temp = p.x / d;

	if (d < 0.0001)
	{
		return 0;
	}

	double value = 0;

	if (p.y < 0)
	{
		value = (acos(-1.0) * 2) - acos(temp);
	}
	else
	{
		value = acos(temp);
	}

	return value;
}


POINT SENC_Instruction::GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset)
{
	double angle = GetAngle(_p1, _p2);  // The angle of the line in the positive direction of the x-axis.

	POINT returnValue;
	returnValue.x = (LONG)(_p1.x + offset * cos(angle));
	returnValue.y = (LONG)(_p1.y + offset * sin(angle));

	return returnValue;
}

Gdiplus::PointF SENC_Instruction::GetOffsetPointFOnLine(POINT _p1, POINT _p2, double offset)
{
	double angle = GetAngle(_p1, _p2);  // The angle of the line in the positive direction of the x-axis.

	PointF returnValue;
	returnValue.X = (float)(_p1.x + offset * cos(angle));
	returnValue.Y = (float)(_p1.y + offset * sin(angle));

	return returnValue;
}

Gdiplus::PointF SENC_Instruction::GetOffsetPointFOnLine(POINT _p1, POINT _p2, double offset, double gapX, double gapY)
{
	double angle = GetAngle(_p1, _p2);  // The angle of the line in the positive direction of the x-axis.

	PointF returnValue;
	returnValue.X = (float)(_p1.x + offset * cos(angle) + gapX);
	returnValue.Y = (float)(_p1.y + offset * sin(angle) + gapY);

	return returnValue;
}

double SENC_Instruction::GetDistanceOfPolyline(POINT *_p, int _count)
{
	double returnValue = 0;

	for (int i = 1; i < _count; i++)
	{
		returnValue += GetDistance(_p[i - 1], _p[i]);
	}

	return returnValue;
}

inline void swap(int &val1, int &val2)
{
	int t = val1;
	val1 = val2;
	val2 = t;
}

bool SENC_Instruction::IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2)
{
	float x12 = (float)(_s1.x - _e1.x);
	float x34 = (float)(_s2.x - _e2.x);
	float y12 = (float)(_s1.y - _e1.y);
	float y34 = (float)(_s2.y - _e2.y);

	float c = x12 * y34 - y12 * x34;

	if (fabs(c) < 0.01)
	{
		// No intersection
		return false;
	}
	else
		return true;
}

void SENC_Instruction::CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort)
{
	// Store the values for fast access and easy

	// equations-to-code conversion
	float x1 = (float)_s1.x, x2 = (float)_e1.x, x3 = (float)_s2.x, x4 = (float)_e2.x;
	float y1 = (float)_s1.y, y2 = (float)_e1.y, y3 = (float)_s2.y, y4 = (float)_e2.y;
	float d = (float)(x1 - x2) * (float)(y3 - y4) - (float)(y1 - y2) * (float)(x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return;

	// Get the x and y
	float pre = (float)(x1*y2 - y1 * x2), post = (float)(x3*y4 - y3 * x4);
	float x = (float)(pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (float)(pre * (y3 - y4) - (y1 - y2) * post) / d;

	if (_s2.x == _s1.x ||
		_s2.x == _s1.x ||
		_s2.y == _s1.y ||
		_s2.y == _s1.y ||
		_e2.x == _e1.x ||
		_e2.x == _e1.x ||
		_e2.y == _e1.y ||
		_e2.y == _e1.y
		)
	{
		return;
	}
	else if (
		viewPort->left > _s1.x ||
		viewPort->right < _s1.x ||
		viewPort->bottom	< _s1.y ||
		viewPort->top	> _s1.y)
	{
		_s1.x = (LONG)x;
		_s1.y = (LONG)y;
	}
	else
	{
		_e1.x = (LONG)x;
		_e1.y = (LONG)y;
	}
}

void SENC_Instruction::GetViewLineInPolyline(POINT *_p, int _count, CRect* viewPort)
{
	POINT p1, p2;
	p1.x = viewPort->left;
	p1.y = viewPort->top;
	p2.x = viewPort->right;
	p2.y = viewPort->top;
	if (p1.y > _p[0].y)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.y > _p[_count - 1].y)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}

	p1.x = viewPort->right;
	p1.y = viewPort->top;
	p2.x = viewPort->right;
	p2.y = viewPort->bottom;
	if (p1.x < _p[0].x)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.x < _p[_count - 1].x)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}
	p1.x = viewPort->left;
	p1.y = viewPort->bottom;
	p2.x = viewPort->right;
	p2.y = viewPort->bottom;
	if (p1.y < _p[0].y)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.y < _p[_count - 1].y)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}
	p1.x = viewPort->left;
	p1.y = viewPort->top;
	p2.x = viewPort->left;
	p2.y = viewPort->bottom;
	if (p1.x > _p[0].x)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	else if (p1.x > _p[_count - 1].x)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}

	return;
}

POINT* SENC_Instruction::GetCenterPointOfPolyline(POINT *_p, int _count, CRect* viewPort)
{
	if (_count < 2)
	{
		return NULL;
	}
	else
	{
		GetViewLineInPolyline(_p, _count, viewPort);
	}

	double centerDistance = GetDistanceOfPolyline(_p, _count) / 2; // The distance from the starting point of the polyline to the middle point of the polyline.

	if (centerDistance == 0)
	{
		return NULL;
	}

	double accumulatedDistance = 0;								   // Accumulated distance.
	POINT *returnValue = new POINT;

	for (int i = 0; i < (_count - 1); i++)						   // Measure the cumulative distance from the 0th point to the next point.
	{
		accumulatedDistance += GetDistance(_p[i], _p[i + 1]);      // Measure the cumulative distance while moving to a different point.

		if (accumulatedDistance >= centerDistance)				   // If the cumulative distance is greater than the intermediate distance, it has passed the midpoint.
		{
			*returnValue = GetOffsetPointOnLine(_p[i + 1], _p[i], accumulatedDistance - centerDistance); // It goes as far as the distance passed.
			break;
		}
	}

	return returnValue;
}

SENC_Instruction* SENC_Instruction::S1002SENC(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output, S100SpatialObject* so)
{
	SENC_Instruction *result = nullptr;

	switch (s100Instruction->GetType())
	{
	case 0: // NullInstruction
		result = new SENC_NullInstruction();
		break;
	case 1: // PointInstruction
		result = new SENC_PointInstruction();
		break;
	case 2: // LineInstruction
		result = new SENC_LineInstruction();
		break;
	case 3: // AreaInstruction
		result = new SENC_AreaInstruction();
		break;
	case 4: // CoverageInstruction
		break;
	case 5: // TextInstruction
		result = new SENC_TextInstruction();
		break;
	case 6: // AugmentedPoint
		break;
	case 7: // AugmentedRay
		result = new SENC_AugmentedRay();
		break;
	case 8: // AugmentedPath
		result = new SENC_AugmentedPath();
		break;
	case 9: // AugmentedArea
		break;
	}

	if (nullptr != result)
	{
		result->FromS100Instruction(s100Instruction, pc, output);
		result->code = so->GetFeatureTypeCodeByID(result->featureReference);
	}
	return result;
}