#pragma once
#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include "..\\extlibs\\Clipper\\include\\clipper.hpp"

class SCommonFuction
{
public:
	/*
	** function   : Clip the given surface to fit the screen area and obtain the center of gravity point of the result.
	** factor   :
	SSurface *_surface  - AREA to find the center of gravity
	CRect*   _viewPort - Screen area.
	** return value :
	NULL  - If the screen and the surface do not overlap at all (the center of gravity cannot exist).
	!NULL - Surface containing center of gravity points (unconditionally, there is one part, and all points are center of gravity points).
	*/
	//static SSurface* CalculateCenterOfGravityOfSurface(SSurface *_surface, CRect *_viewPort);
	static void CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler);

	/*
	** function   : Clips the given AREA to fit the screen area.
	** factor   :
	ENCArea* _polygon  - Area to clip.
	CRect*   _viewPort - Screen area.
	** return value :
	NULL  - If the screen area and AREA do not overlap at all,
	!NULL - As a result of clipping, it may be a multi-part.
	*/
	static ClipperLib::Paths ClipSurface(SSurface *_polygon, CRect *_viewPort);


	/*
	** function   : Find the center of gravity of a given polygon.
	** factor   :
	POINT* _p     - The arrangement with the coordinates of polygon.
	int    _count - The number of polygon coordinates.
	** return value : The center of gravity.
	*/
	static POINT CalculateCenterOfGravityOfSurface(POINT *_p, int _count);


	/*
	** function   : Find the center of gravity of a given polygon.
	** factor   :
	POINT* _p     - The arrangement with the coordinates of polygon.
	int    _count - The number of polygon coordinates.
	** return value : The center of gravity.
	*/
	static POINT CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon);


	/*
	** function   : Find the distance between the two points.
	** factor   :
	POINT _p1 - The first point.
	POINT _p2 - The second point.
	** return value : The distance between the two points.
	*/
	static double GetDistance(POINT _p1, POINT _p2);


	/*
	** function   : Find the length of the polyline.
	** factor   :
	POINT* _p     - Coordinate arrangement of the polyline.
	int    _count - The number of polyline coordinates.
	** return value : The length of the polyline.
	*/
	static double GetDistanceOfCurve(POINT *_p, int _count);

	/*
	** function   : Find the line shown on the screen among the polylines.
	** factor   :
	POINT* _p     - Coordinate arrangement of the polyline.
	int    _count - The number of polyline coordinates.
	** return value : Among the polylines, the line you see on the screen.
	*/
	static void GetViewLineInCurve(POINT *_p, int _count, CRect* viewPort);

	/*
	** function   : Find the midpoint at the distance of the polyline.
	** factor   :
	POINT* _p     - Coordinate arrangement of the polyline.
	int    _count -The number of polyline coordinates.
	** return value : The midpoint of the polyline.
	*/
	static POINT* GetCenterPointOfCurve(POINT *_p, int _count, CRect* viewPort);

	/*
	** function   : Check if the two line segments intersect.
	** factor   :
	POINT _s1     - The starting point of LINE 1.
	POINT _e1     - The end point of Line 1.
	POINT _s2     - The starting point of Line 2.
	POINT _e2     - The end point of Line 2.
	** return value : Whether the two lines intersect.
	*/
	static bool IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2);

	/*
	** function   :
	** factor   : Move the starting point of the line from the intersection.
	POINT _s1     - The starting point of LINE 1.
	POINT _e1     - The end point of Line 1.
	POINT _s2     - The starting point of LINE 2.
	POINT _e2     - The end point of Line 2.
	** return value :
	*/
	static void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort);


	/*
	** function   : Find the angle in the positive direction between the two-dimensional vector and the x-axis.
	** factor   :
	POINT _p1 - The starting point of the vector.
	POINT _p2 - The end point of a vector.
	** return value : Result value (unit: radian, range: 0 to 2 * pi)
	*/
	static double GetAngle(POINT _p1, POINT _p2);


	/*
	** function   : Find a point (distance) away from the starting point of the line by offset.
	** factor   :
	POINT _p1     - The starting point of the line.
	POINT _p2     - The end point of the line.
	double offset - offset
	** return value : The point that is offset from the starting point of the line.
	*/
	static POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);


	///*
	//** function : Draw a composite line on the line.
	//** factor :
	//CDC*   pDC     - DC for drawing.
	//POINT _p1      - The starting point of the line.
	//POINT _p2      - The end point of the line.
	//Symbol _symbol - CompositeLine
	//*/
	//static void DrawCompositeLineOnLine(CDC *pDC, POINT _p1, POINT _p2, Symbol _symbol);


	///*
	//** function : Draw the composite line on the curve.
	//** factor :
	//CDC*   pDC     - DC for drawing.
	//POINT  _p1     - The starting point of the line.
	//POINT  _p2     - The end point of the line.
	//Symbol _symbol - CompositeLine
	//*/
	//static void DrawCompositeLineOnCurve(CDC *pDC, POINT *_p, int _count, Symbol _symbol);

	// line & line
	static bool Intersect(
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4,
		float* intersectionX, float* intersectionY);

	// line & rect
	static bool Intersect(
		float xmin, float ymin, float xmax, float ymax,
		float x1, float y1, float x2, float y2,
		float& intersectionX1, float& intersectionY1,
		float& intersectionX2, float& intersectionY2);

	//static const int NewGeometryScale = 70000000;
	static const int NewGeometryScale = 5000;

	static int intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y);

	// -1 : counterclockwise
	//  0 : straight line
	//  1 : clockwise
	static int ccw(double ax, double ay, double bx, double by, double cx, double cy);

	static int overlap(SCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SCompositeCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SSurface* objPoly, SSurface* comPoly, bool applyOption = false);
	// for S101
	static int inside(SPoint* point, SSurface* poly, bool applyOption = false);
	static int inside(double x, double y, SSurface* poly, bool applyOption = false);
	static double GetAngle(double p1x, double p1y, double p2x, double p2y);
};