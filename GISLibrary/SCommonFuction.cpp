#include "stdafx.h"
#include "SCommonFuction.h"
#include "SPoint.h"
#include "SCurve.h"
#include "SSurface.h"
#include "GISLibrary.h"

#include "../GeoMetryLibrary/Scaler.h"

void SCommonFuction::CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler)
{
	std::vector<POINT> m_pvPoints;

	if (_surface->getNumPoint() > SGeometry::sizeOfPoint)
	{
		SGeometry::sizeOfPoint = (int)(_surface->getNumPoint() * 1.5);

		delete[] SGeometry::viewPoints;
		SGeometry::viewPoints = new CPoint[SGeometry::sizeOfPoint];
	}

	for (int count = 0; count < _surface->getNumPoint(); count++)
	{
		pScaler->WorldToDevice(_surface->m_pPoints[count].x, _surface->m_pPoints[count].y, &SGeometry::viewPoints[count].x, &SGeometry::viewPoints[count].y);
	}

	ClipperLib::Paths clipedPolygon = ClipSurface(_surface, _viewPort); // result of clipping the polygon that came as a factor,

	if (clipedPolygon.size() == 0)
	{
		return;
	}

	int indexOfMaxVertexPolygon = 0;
	int curIndex = 0;
	int maxVertex = 0;
	for (auto itor = clipedPolygon.begin(); itor != clipedPolygon.end(); itor++)
	{
		auto polygon = *itor;
		
		int curVertex = (int)polygon.size();

		if (curVertex > maxVertex)
		{
			indexOfMaxVertexPolygon = curIndex;
			maxVertex = curVertex;
		}
		curIndex++;
	}

	curIndex = 0;
	for (auto itor = clipedPolygon.begin(); itor != clipedPolygon.end(); itor++)
	{
		if (curIndex == indexOfMaxVertexPolygon)
		{
			vp.push_back(CalculateCenterOfGravityOfSurface(*itor));
		}
		curIndex++;
	}

	return;
}

ClipperLib::Paths SCommonFuction::ClipPaths(const ClipperLib::Paths& viewportPaths, const ClipperLib::Paths& itemPolygonPath) {
	ClipperLib::Clipper c;
	ClipperLib::Paths solution;

	c.AddPaths(viewportPaths, ClipperLib::ptSubject, true);
	c.AddPaths(itemPolygonPath, ClipperLib::ptClip, true);

	c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	return solution;
}


ClipperLib::Paths SCommonFuction::ClipPaths(const ClipperLib::Paths& viewportPaths, std::vector<D2D1_POINT_2F>& itemPolygonPath, Scaler* scaler) {
	ClipperLib::Clipper c;
	ClipperLib::Paths solution;
	ClipperLib::Path  polygon;
	ClipperLib::Paths polygonPaths;
	ClipperLib::IntPoint tmp;


	int part1PointCount = itemPolygonPath.size();											// Find the number of points for the first part of the polygon.
	//
	long x, y;
	
	for (int i = 0; i < part1PointCount; i++)
	{
		scaler->WorldToDevice(itemPolygonPath[i].x, itemPolygonPath[i].y, &x, &y);
		tmp.X = x;
		tmp.Y = y;
		polygon.push_back(tmp);
	}

	polygonPaths.push_back(polygon);

	c.AddPaths(viewportPaths, ClipperLib::ptSubject, true);
	c.AddPaths(polygonPaths, ClipperLib::ptClip, true);

	c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	return solution;
}



ClipperLib::Paths SCommonFuction::ClipPaths(const ClipperLib::Paths& viewportPaths, SSurface* itemPolygonPath) {
	ClipperLib::Clipper clipper;
	ClipperLib::Path  polygon;
	ClipperLib::Paths solution;
	ClipperLib::Paths polygonPaths;

	ClipperLib::IntPoint tmp;

	// Polygon
	int part1PointCount;											// Find the number of points for the first part of the polygon.
	//
	if (itemPolygonPath->GetNumPart() > 1) 									//
	{																//
		part1PointCount = itemPolygonPath->GetNumPointPerPart(0);
	}																//
	else 															//
	{																//
		part1PointCount = itemPolygonPath->getNumPoint();						//
	}																//

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = itemPolygonPath->m_pPoints[i].x;
		tmp.Y = itemPolygonPath->m_pPoints[i].y;
		polygon.push_back(tmp);
	}

	polygonPaths.push_back(polygon);

	clipper.AddPaths(viewportPaths, ClipperLib::ptSubject, true);
	clipper.AddPaths(polygonPaths, ClipperLib::ptClip, true);

	clipper.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	return solution;
}

bool SCommonFuction::IntersectionPaths(ClipperLib::Paths& viewport, std::vector<D2D1_POINT_2F>& polygon, Scaler* scaler)
{
	ClipperLib::Clipper clipper;
	ClipperLib::Paths solution;
	ClipperLib::Path  polygonpath;
	ClipperLib::Paths polygonPaths;
	ClipperLib::IntPoint tmp;


	int part1PointCount = polygon.size();											// Find the number of points for the first part of the polygon.
	long x, y;

	for (int i = 0; i < part1PointCount; i++)
	{
		scaler->WorldToDevice(polygon[i].x, polygon[i].y, &x, &y);
		tmp.X = x;
		tmp.Y = y;
		polygonpath.push_back(tmp);
	}

	polygonPaths.push_back(polygonpath);


	clipper.AddPaths(viewport, ClipperLib::ptSubject, true);
	clipper.AddPaths(polygonPaths, ClipperLib::ptClip, true);

	clipper.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	return !solution.empty();
}

ClipperLib::Paths SCommonFuction::ClipSurface(SSurface *_surface, CRect *_viewPort)
{
	ClipperLib::Path  polygon, view;
	ClipperLib::Paths polygonPaths, viewPaths;
	ClipperLib::Paths result;
	ClipperLib::Clipper clipper;

	ClipperLib::IntPoint tmp;

	// View port
	tmp.X = _viewPort->left;   //Specify the coordinates of the View port
	tmp.Y = _viewPort->top;    //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->right;  //
	tmp.Y = _viewPort->top;	   //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->right;  //
	tmp.Y = _viewPort->bottom; //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->left;   //
	tmp.Y = _viewPort->bottom; //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->left;   //
	tmp.Y = _viewPort->top; //
	view.push_back(tmp);	   //

	// Polygon
	int part1PointCount;											// Find the number of points for the first part of the polygon.
	//
	if (_surface->GetNumPart() > 1) 									//
	{																//
		part1PointCount = _surface->GetNumPointPerPart(0);
	}																//
	else 															//
	{																//
		part1PointCount = _surface->getNumPoint();						//
	}																//

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = _surface->SGeometry::viewPoints[i].x;
		tmp.Y = _surface->SGeometry::viewPoints[i].y;
		polygon.push_back(tmp);
	}

	polygonPaths.push_back(polygon);
	viewPaths.push_back(view);

	clipper.AddPaths(polygonPaths, ClipperLib::ptSubject, true);
	clipper.AddPaths(viewPaths, ClipperLib::ptClip, true);

	clipper.Execute(ClipperLib::ctIntersection, result, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
	
	return result;
}




ClipperLib::Paths SCommonFuction::ClipSurface(SSurface* _surface, SSurface* _viewPort)
{
	ClipperLib::Path  polygon, view;
	ClipperLib::Paths polygonPaths, viewPaths;
	ClipperLib::Paths result;
	ClipperLib::Clipper clipper;

	ClipperLib::IntPoint tmp;

	int part1PointCount;											// Find the number of points for the first part of the polygon.

	// View port
	if (_viewPort->GetNumPart() > 1) 									
	{																
		part1PointCount = _viewPort->GetNumPointPerPart(0);
	}																
	else 															
	{																
		part1PointCount = _viewPort->getNumPoint();					
	}																

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = _viewPort->SGeometry::viewPoints[i].x;
		tmp.Y = _viewPort->SGeometry::viewPoints[i].y;
		view.push_back(tmp);
	}

	// Polygon
	if (_surface->GetNumPart() > 1) 									
	{																
		part1PointCount = _surface->GetNumPointPerPart(0);
	}																
	else 															
	{																
		part1PointCount = _surface->getNumPoint();					
	}																

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = _surface->SGeometry::viewPoints[i].x;
		tmp.Y = _surface->SGeometry::viewPoints[i].y;
		polygon.push_back(tmp);
	}

	polygonPaths.push_back(polygon);
	viewPaths.push_back(view);

	clipper.AddPaths(polygonPaths, ClipperLib::ptSubject, true);
	clipper.AddPaths(viewPaths, ClipperLib::ptClip, true);

	clipper.Execute(ClipperLib::ctIntersection, result, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	return result;
}



POINT SCommonFuction::CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon)
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	ClipperLib::long64 x1, y1, x2, y2, tmpArea;

	int _count = (int)polygon.size();

	for (auto itor = polygon.begin(); itor != polygon.end(); itor++)
	{
		auto itor2 = itor + 1;
		if (itor2 == polygon.end())
			itor2 = polygon.begin();

		x1 = (*itor).X;
		y1 = (*itor).Y;
		x2 = (*itor2).X;
		y2 = (*itor2).Y;

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

double SCommonFuction::GetDistance(POINT _p1, POINT _p2)
{
	POINT p;

	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	return sqrt((double)((p.x * p.x) + (p.y * p.y)));
}


POINT SCommonFuction::GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset)
{
	double angle = GetAngle(_p1, _p2);  // The angle of the line in the positive direction of the x-axis.

	POINT returnValue;
	returnValue.x = (LONG)(_p1.x + offset * cos(angle));
	returnValue.y = (LONG)(_p1.y + offset * sin(angle));

	return returnValue;
}

double SCommonFuction::GetDistanceOfCurve(POINT *_p, int _count)
{
	double returnValue = 0;

	for (int i = 1; i < _count; i++)
	{
		returnValue += GetDistance(_p[i - 1], _p[i]);
	}

	return returnValue;
}

void SCommonFuction::CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort)
{
	// Store the values for fast access and easy

	// equations-to-code conversion
	float x1 = (float)_s1.x, x2 = (float)_e1.x, x3 = (float)_s2.x, x4 = (float)_e2.x;
	float y1 = (float)_s1.y, y2 = (float)_e1.y, y3 = (float)_s2.y, y4 = (float)_e2.y;
	float d = (float)(x1 - x2) * (float)(y3 - y4) - (float)(y1 - y2) * (float)(x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return;

	// Get the x and y
	float pre = (float)(x1*y2 - y1*x2), post = (float)(x3*y4 - y3*x4);
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
		viewPort->left	> _s1.x ||
		viewPort->right	< _s1.x ||
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

void SCommonFuction::GetViewLineInCurve(POINT *_p, int _count, CRect* viewPort)
{
	POINT p1, p2;
	p1.x =viewPort->left;
	p1.y =viewPort->top;
	p2.x =viewPort->right;
	p2.y =viewPort->top;
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

POINT* SCommonFuction::GetCenterPointOfCurve(POINT *_p, int _count, CRect* viewPort)
{
	if (_count < 2)
	{
		return NULL;
	}
	else
	{
		GetViewLineInCurve(_p, _count, viewPort);
	}

	double centerDistance = GetDistanceOfCurve(_p, _count) / 2; // The distance from the starting point of the polyline to the middle point of the polyline.

	if (centerDistance == 0)
	{
		return NULL;
	}

	double accumulatedDistance = 0;								   // Accumulated distance.
	POINT *returnValue = new POINT;

	for (int i = 0; i < (_count - 1); i++)						   // Measure the cumulative distance from the 0th point to the next point.
	{
		accumulatedDistance += GetDistance(_p[i], _p[i + 1]);      // Move to the next point and measure the cumulative distance.

		if (accumulatedDistance >= centerDistance)				   // If the cumulative distance is greater than the intermediate distance, it has passed the midpoint.
		{
			*returnValue = GetOffsetPointOnLine(_p[i + 1], _p[i], accumulatedDistance - centerDistance); // It goes as far as the distance passed.
			break;
		}
	}

	return returnValue;
}

//  -1 : outside
//  1 : inside
int SCommonFuction::inside(double x, double y, SSurface* poly, bool applyOption)
{
	if (poly == NULL)
		return -1;
	int i;
	int count = 0;
	int ic;
	count = 0;

	int lastExteriorIdx = poly->getLastPointIndexOfExterior();

	int obj2PartIndex = 0;

	int entryIc = -1;


	//
	int* icList = NULL;
	static int ticList[100000];
	if (poly->getNumPoint() > 100000)
	{
		int* ticList = new int[poly->getNumPoint()];
		icList = ticList;
	}
	else
	{
		icList = ticList;
	}


	int listIndex = 0;
	//
	for (i = 0; i < poly->getNumPoint() - 1; i++)
	{
		if (poly->GetNumPart() > obj2PartIndex + 1)
		{
			if (poly->getPart(obj2PartIndex + 1) == i + 1)
			{
				obj2PartIndex++;
				continue;
			}
		}

		double max_px = max(max(poly->m_pPoints[i].x, poly->m_pPoints[i + 1].x) + 1, x + 1);

		ic = intersect_ccw(x, y, max_px, y, poly->m_pPoints[i].x, poly->m_pPoints[i].y, poly->m_pPoints[i + 1].x, poly->m_pPoints[i + 1].y);
		icList[listIndex++] = ic;

		if (ic == -1)
		{
		}
		if (ic == 0)
		{
			if (x > min(poly->m_pPoints[i].x, poly->m_pPoints[i + 1].x))
			{
				count = 0;
				break;
			}
		}
		else if (ic == 1)
		{
			count++;
		}
		else if (ic >= 20 || ic == 12)
		{
			count = 0;
			break;
		}
		else if (ic == 10) // Go counterclockwise (in/out)
		{
			if (entryIc == 11)
			{
				count++;
				entryIc = -1;
			}
			else if (lastExteriorIdx < i)
			{
				entryIc = ic;
			}
			else
				entryIc = ic;
		}
		else if (ic == 11) // Clockwise (in/out)
		{
			if (entryIc == 10)
			{
				count++;
				entryIc = -1;
			}
			else if (lastExteriorIdx < i)
			{
				entryIc = ic;
			}
			else
				entryIc = ic;
		}
	}
	if (count & 1)
		return 1;
	else
		return -1;
}

double SCommonFuction::GetAngle(POINT _p1, POINT _p2)
{
	POINT p;
	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	double d = sqrt(((double)(p.x) * p.x) + (p.y * p.y));
	double temp = p.x / d;

	if (p.y < 0)
	{
		return (acos(-1.0) * 2) - acos(temp);
	}

	return acos(temp);
}

int SCommonFuction::overlap(SCompositeCurve* objPoly, SSurface* comPoly, bool applyOption)
{
	int curveCnt = objPoly->GetCurveCount();
	for (int i = 0; i < curveCnt; i++)
	{
		auto c = objPoly->GetCurve(i);

		int result = overlap(c, comPoly, applyOption);
		if (result)
		{
			return result;
		}
	}
	return -1;
}

int SCommonFuction::overlap(SAbstractCurve* objPoly, SSurface* comPoly, bool applyOption)
{
	if (objPoly->GetType() == SGeometryType::Curve) {
		auto curve = (SCurve*)objPoly;
		return overlap(curve, comPoly, applyOption);
	}
	else if (objPoly->GetType() == SGeometryType::CompositeCurve) {
		auto curve = (SCompositeCurve*)objPoly;
		return overlap(curve, comPoly, applyOption);
	}

	return -1;
}

int SCommonFuction::overlap(SCurve* objPoly, SSurface* comPoly, bool applyOption)
{

	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	for (i = 0; i < objPoly->m_numPoints - 1 && ret != 1; i++)
	{
		for (j = 0; j < comPoly->getNumPoint() - 1 && ret != 1; j++)
		{
			ret = intersect_ccw(objPoly->m_pPoints[i].x, objPoly->m_pPoints[i].y, objPoly->m_pPoints[i + 1].x, objPoly->m_pPoints[i + 1].y,
				comPoly->m_pPoints[j].x, comPoly->m_pPoints[j].y, comPoly->m_pPoints[j + 1].x, comPoly->m_pPoints[j + 1].y);
		}
	}

	if (ret <= 0)
	{
		for (j = 0; j < comPoly->getNumPoint() - 1 && ret <= 0; j++)
		{
			ret = inside(objPoly->m_pPoints[j].x, objPoly->m_pPoints[j].y, comPoly);
		}
	}

	return ret;
}

int SCommonFuction::overlap(SSurface* objPoly, SSurface* comPoly, bool applyOption)
{
	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	for (i = 0; i < objPoly->getNumPoint() - 1 && ret != 1; i++)
	{
		for (j = 0; j < comPoly->getNumPoint() - 1 && ret != 1; j++)
		{
			ret = intersect_ccw(objPoly->m_pPoints[i].x, objPoly->m_pPoints[i].y, objPoly->m_pPoints[i + 1].x, objPoly->m_pPoints[i + 1].y,
				comPoly->m_pPoints[j].x, comPoly->m_pPoints[j].y, comPoly->m_pPoints[j + 1].x, comPoly->m_pPoints[j + 1].y);
		}
	}

	if (ret <= 0)
	{
		for (j = 0; j < comPoly->getNumPoint() - 1 && ret <= 0; j++)
		{
			ret = inside(objPoly->m_pPoints[j].x, objPoly->m_pPoints[j].y, comPoly);
		}
	}

	return ret;
}

void swap_double_value(double* var1, double* var2)
{
	double temp = *var1;
	*var1 = *var2;
	*var2 = temp;
}
int SCommonFuction::intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y)
{
	int r123, r124, r341, r342;

	if (p1x > p2x)
	{
		swap_double_value(&p1x, &p2x);
		swap_double_value(&p1y, &p2y);
	}
	if (p3x > p4x)
	{
		swap_double_value(&p3x, &p4x);
		swap_double_value(&p3y, &p4y);
	}

	r123 = ccw(p1x, p1y, p2x, p2y, p3x, p3y);
	r124 = ccw(p1x, p1y, p2x, p2y, p4x, p4y);
	r341 = ccw(p3x, p3y, p4x, p4y, p1x, p1y);
	r342 = ccw(p3x, p3y, p4x, p4y, p2x, p2y);

	if (p1x == p2x && p1y == p2y)
	{
		if (341 * 342 == 0)
			return 1;
		else
			return -1;
	}
	else if (p3x == p4x && p3y == p4y)
	{
		if (123 * 124 == 0)
			return 1;
		else
			return -1;
	}

	if ((r123 * r124 < 0) && (r341 * r342 < 0))
		return 1; // Cross.

	if (r123 == 0 && r124 == 0) // directions same
	{
		if (p3x > p2x || p1x > p4x)
			return -1;	// directions not same
	}

	if (r341 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
	{
		if ((p3x == p1x && p3y == p1y)
			|| (p4x == p1x && p4y == p1y))
		{
			return 22;
		}
		else if (p3x <= p1x && p1x <= p4x && min(p3y, p4y) <= p1y && max(p3y, p4y) >= p1y)
		{
			if (r342 == 1) // clockwise
			{
				return 20;
			}
			else if (r342 == -1) // Counterclockwise
			{
				return 21;
			}
			else  // cross
				return 0;
		}
		else
		{
			if (r342 == 1) // clockwise
			{
				return -20;
			}
			else if (r342 == -1) // Counterclockwise
			{
				return -21;
			}
			else// cross
				return 0;
		}
	}

	if (r342 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
	{
		if ((p3x == p2x && p3y == p2y)
			|| (p4x == p2x && p4y == p2y))
		{
			return 22;
		}
		else if (p3x <= p2x && p2x <= p4x && min(p3y, p4y) <= p2y && max(p3y, p4y) >= p2y)
		{
			if (r341 == 1) // clockwise
			{
				return 20;
			}
			else if (r341 == -1) // Counterclockwise
			{
				return 21;
			}
			else// cross
				return 0;
		}
		else  // If there's an end point on the extension,
		{
			if (r341 == 1) // clockwise
			{
				return -20;
			}
			else if (r341 == -1) // Counterclockwise
			{
				return -21;
			}
			else// cross
				return 0;
		}
	}
	if (r123 == 0) // If it's inside the line segment or extension in the line segment starting direction,
	{
		if ((p1x == p3x && p1y == p3y)
			|| (p2x == p3x && p2y == p3y))
		{
			return 12;
		}
		else if (p1x <= p3x && p3x <= p2x && min(p1y, p2y) <= p3y && max(p1y, p2y) >= p3y)
		{
			if (r124 == 1) // clockwise
			{
				return 10;
			}
			else if (r124 == -1) // Counterclockwise
			{
				return 11;
			}
			else// cross
				return 0;
		}
		else
		{
			if (r124 == 1) // clockwise
			{
				return -10;
			}
			else if (r124 == -1) // Counterclockwise
			{
				return -11;
			}
			else// cross
				return 0;
		}
	}
	if (r124 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
	{
		if ((p1x == p4x && p1y == p4y)
			|| (p2x == p4x && p2y == p4y))
		{
			return 12;
		}
		else if (p1x <= p4x && p4x <= p2x && min(p1y, p2y) <= p4y && max(p1y, p2y) >= p4y)
		{
			if (r123 == 1) // clockwise
			{
				return 10;
			}
			else if (r123 == -1) // Counterclockwise
			{
				return 11;
			}
			else// cross
				return 0;
		}
		else // Extension.
		{
			if (r123 == 1) // clockwise
			{
				return -10;
			}
			else if (r123 == -1) // Counterclockwise 
			{
				return -11;
			}
			else// cross
				return 0;
		}
	}

	return -1; // not cross
}


// -1 : Counterclockwise
//  0 : Straight line
//  1 : clockwise
int SCommonFuction::ccw(double ax, double ay, double bx, double by, double cx, double cy)
{
	double l;
	l = bx * cy - ay * bx - ax * cy
		- by * cx + ax * by + ay * cx;

	if (l > 0)
		return 1; // cw 
	else if (l < 0)
		return -1; // ccw 
	else
		return 0; // Straight line
}