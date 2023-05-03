#pragma once
#include "SPoint.h"
#include "SCurve.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include "../GeoMetryLibrary/GeometricFuc.h"

class SGeometricFuc :
	public GeometricFuc
{
public:
	// for S101
	static int overlap(double p1x, double p1y, SCurve *comPoly);
	static int overlap(SPoint *point, SCurve *comPoly);
	static int overlap(SCurve *objPoly, SSurface *comPoly); // Used for curve Pick Report
	static int overlap(SCompositeCurve *objPoly, SSurface *comPoly); // Used for composite curve Pick Report
	static int overlap(SSurface *objPoly, SSurface *comPoly);

	// for S101
	static int inside(SPoint *point, SSurface *poly);
	static int inside(double x, double y, SSurface *poly); // Used for surface Pick Report
	static double GetAngle(POINT _p1, POINT _p2);
	static double GetAngle(double p1x, double p1y, double p2x, double p2y);
};