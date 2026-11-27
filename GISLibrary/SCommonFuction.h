#pragma once
#include "../GeoMetryLibrary/ScreenTypes.h"
#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include <polyclipping/clipper.hpp>

class SCommonFuction
{
public:
	/*
	** function   : Clip the given surface to fit the screen area and obtain the center of gravity point of the result.
	** factor   :
	SSurface *_surface  - AREA to find the center of gravity
	const ScreenRect* _viewPort - Screen area.
	** return value :
	NULL  - If the screen and the surface do not overlap at all (the center of gravity cannot exist).
	!NULL - Surface containing center of gravity points (unconditionally, there is one part, and all points are center of gravity points).
	*/
	static void CalculateCenterOfGravityOfSurface(std::vector<LongPoint> &vp, SSurface *_surface, const ScreenRect* _viewPort, Scaler *pScaler);

	static ClipperLib::Paths ClipPaths(const ClipperLib::Paths& viewportPaths, const ClipperLib::Paths& itemPolygonPath);
	static ClipperLib::Paths ClipPaths(const ClipperLib::Paths& viewportPaths, SSurface* itemPolygonPath);
	

	/*
	** function   : Clips the given AREA to fit the screen area.
	** factor   :
	ENCArea* _polygon  - Area to clip.
	const ScreenRect* _viewPort - Screen area.
	** return value :
	NULL  - If the screen area and AREA do not overlap at all,
	!NULL - As a result of clipping, it may be a multi-part.
	*/
	static ClipperLib::Paths ClipSurface(SSurface *_polygon, const ScreenRect* _viewPort);

	/*
	** function   : Clips the given AREA to fit the screen area.
	** factor   :
	SSurface* _polygon  - Area to clip.
	SSurface*   _viewPort - Screen area.
	** return value :
	NULL  - If the screen area and AREA do not overlap at all,
	!NULL - As a result of clipping, it may be a multi-part.
	*/
	static ClipperLib::Paths ClipSurface(SSurface* _surface, SSurface* _viewPort);

	/*
	** function   : Find the center of gravity of a given polygon.
	** factor   :
	LongPoint* _p     - The arrangement with the coordinates of polygon.
	int    _count - The number of polygon coordinates.
	** return value : The center of gravity.
	*/
	static LongPoint CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon);


	/*
	** function   : Find the distance between the two points.
	** factor   :
	LongPoint _p1 - The first point.
	LongPoint _p2 - The second point.
	** return value : The distance between the two points.
	*/
	static double GetDistance(LongPoint _p1, LongPoint _p2);


	/*
	** function   : Find the length of the polyline.
	** factor   :
	LongPoint* _p     - Coordinate arrangement of the polyline.
	int    _count - The number of polyline coordinates.
	** return value : The length of the polyline.
	*/
	static double GetDistanceOfCurve(LongPoint *_p, int _count);

	/*
	** function   : Find the line shown on the screen among the polylines.
	** factor   :
	LongPoint* _p     - Coordinate arrangement of the polyline.
	int    _count - The number of polyline coordinates.
	** return value : Among the polylines, the line you see on the screen.
	*/
	static void GetViewLineInCurve(LongPoint *_p, int _count, const ScreenRect* viewPort);

	/*
	** function   : Find the midpoint at the distance of the polyline.
	** factor   :
	LongPoint* _p     - Coordinate arrangement of the polyline.
	int    _count -The number of polyline coordinates.
	** return value : The midpoint of the polyline.
	*/
	static LongPoint* GetCenterPointOfCurve(LongPoint *_p, int _count, const ScreenRect* viewPort);

	/*
	** function   :
	** factor   : Move the starting point of the line from the intersection.
	LongPoint _s1     - The starting point of LINE 1.
	LongPoint _e1     - The end point of Line 1.
	LongPoint _s2     - The starting point of LINE 2.
	LongPoint _e2     - The end point of Line 2.
	** return value :
	*/
	static void CutLineToIntersect(LongPoint &_s1, LongPoint &_e1, LongPoint _s2, LongPoint _e2, const ScreenRect* viewPort);


	/*
	** function   : Find the angle in the positive direction between the two-dimensional vector and the x-axis.
	** factor   :
	LongPoint _p1 - The starting point of the vector.
	LongPoint _p2 - The end point of a vector.
	** return value : Result value (unit: radian, range: 0 to 2 * pi)
	*/
	static double GetAngle(LongPoint _p1, LongPoint _p2);


	/*
	** function   : Find a point (distance) away from the starting point of the line by offset.
	** factor   :
	LongPoint _p1     - The starting point of the line.
	LongPoint _p2     - The end point of the line.
	double offset - offset
	** return value : The point that is offset from the starting point of the line.
	*/
	static LongPoint GetOffsetPointOnLine(LongPoint _p1, LongPoint _p2, double offset);

	//static const int NewGeometryScale = 70000000;
	static const int NewGeometryScale = 5000;

	static int intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y);

	// -1 : counterclockwise
	//  0 : straight line
	//  1 : clockwise
	static int ccw(double ax, double ay, double bx, double by, double cx, double cy);

	static int overlap(SAbstractCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SCompositeCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SSurface* objPoly, SSurface* comPoly, bool applyOption = false);
	// for S101
	static int inside(double x, double y, SSurface* poly, bool applyOption = false);
};