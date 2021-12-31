#pragma once

class ENCPoint;
class GeoPolygon;
class GeoPolyline;
class GeoPoint;
class ENCPoint;
class ENCSounding;
class ENCArea;
class SPoint;
class SCurve;
class SSurface;
class GeometricFuc
{
public:
	GeometricFuc(void);
	virtual ~GeometricFuc(void);

public:
	static int intersect_equation(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y);

	// -1 : not cross
	//  0 : If there's an end point of a line inside the line segment,
	//  1 : cross
	static int intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y);
	static int intersect_ccw(GeoPolyline *objPoly, GeoPolygon *comPoly);
	static int intersect_ccw(GeoPolyline *objPoly, GeoPolyline *objPoly2);

	// -1 : Counterclockwise
	//  0 : Straight
	//  1 : clockwise
	static int ccw(double ax, double ay, double bx, double by, double cx, double cy);
	/*
		Overlap must have two pieces of spatial information inside.Because
		In the case of Area, Lineand Point on the Outer line or the Outer line on the other area,
		Overlap is not the case if it only overlapsand there is no overlap with Interior.
	*/
	// -1 : No Overlap
	//  1 : Overlap
	static int overlap(double p1x, double p1y, GeoPolyline *comPoly, bool applyOption = false);
	static int overlap(GeoPolyline *objPoly, GeoPolyline *comPoly, bool applyOption = false);
	static int overlap(GeoPolyline *objPoly, GeoPolygon *comPoly, bool applyOption = false);
	static int overlap(GeoPolygon *objPoly, GeoPolygon *comPoly, bool applyOption = false);

	//  -1 : outside
	//  1 : inside
	static int inside(GeoPoint *point, GeoPolygon *poly, bool applyOption = false);
	static int inside(double x, double y, GeoPolygon *poly, bool applyOption = false);
};

void swap(double *var1, double *var2);