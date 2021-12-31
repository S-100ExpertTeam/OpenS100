#pragma once
#include "SPoint.h"
#include "SCurve.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include "..\\GeoMetryLibrary\\GeometricFuc.h"

class SGeometricFuc :
	public GeometricFuc
{
public:
	// for S101
	static int overlap(double p1x, double p1y, SCurve *comPoly, bool applyOption = false);
	static int overlap(SPoint *point, SCurve *comPoly, bool applyOption = false);
	static int overlap(SCurve *objPoly, SSurface *comPoly, bool applyOption = false);
	static int overlap(SCompositeCurve *objPoly, SSurface *comPoly, bool applyOption = false);
	static int overlap(SSurface *objPoly, SSurface *comPoly, bool applyOption = false);

	// for S101
	static int inside(SPoint *point, SSurface *poly, bool applyOption = false);
	static int inside(double x, double y, SSurface *poly, bool applyOption = false);
	static double GetAngle(POINT _p1, POINT _p2);
	static double GetAngle(double p1x, double p1y, double p2x, double p2y);
};