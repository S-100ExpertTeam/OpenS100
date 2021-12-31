#include "stdafx.h"
#include "SGeometricFuc.h"

#include "..\\GeoMetryLibrary\\ENCCommon.h"

int SGeometricFuc::overlap(SCurve *objPoly, SSurface *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}

	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	for (i = 0; i < objPoly->m_numPoints - 1 && ret != 1; i++)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret != 1; j++)
		{
			auto dsf = objPoly->m_pPoints[i].x;

			ret = GeometricFuc::intersect_ccw(objPoly->m_pPoints[i].x, objPoly->m_pPoints[i].y, objPoly->m_pPoints[i + 1].x, objPoly->m_pPoints[i + 1].y,
				comPoly->m_pPoints[j].x, comPoly->m_pPoints[j].y, comPoly->m_pPoints[j + 1].x, comPoly->m_pPoints[j + 1].y);
		}
	}

	if (ret <= 0)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret <= 0; j++)
		{
			ret = inside(objPoly->m_pPoints[j].x, objPoly->m_pPoints[j].y, comPoly);
		}
	}

	return ret;
}


int SGeometricFuc::overlap(SCompositeCurve *objPoly, SSurface *comPoly, bool applyOption)
{

	if (applyOption)
	{
		return -1;
	}

	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}


	for (auto it = objPoly->m_listCurveLink.begin(); it != objPoly->m_listCurveLink.end(); it++)
	{
		SCurve* c = (*it).GetCurve();

		int fret = overlap(c, comPoly, applyOption);

		if (fret != -1)
		{
			ret = fret;
		}
	}
	
	return ret;
}

int SGeometricFuc::overlap(SSurface *objPoly, SSurface *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}
	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	for (i = 0; i < objPoly->m_numPoints - 1 && ret != 1; i++)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret != 1; j++)
		{
			ret = GeometricFuc::intersect_ccw(objPoly->m_pPoints[i].x, objPoly->m_pPoints[i].y, objPoly->m_pPoints[i + 1].x, objPoly->m_pPoints[i + 1].y,
				comPoly->m_pPoints[j].x, comPoly->m_pPoints[j].y, comPoly->m_pPoints[j + 1].x, comPoly->m_pPoints[j + 1].y);
		}
	}

	if (ret <= 0)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret <= 0; j++)
		{
			ret = inside(objPoly->m_pPoints[j].x, objPoly->m_pPoints[j].y, comPoly);
		}
	}

	return ret;
}
int SGeometricFuc::overlap(double p1x, double p1y, SCurve *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}
	int r341;

	int ret = -1;

	MBR p_mbr;
	p_mbr.xmax = p1x;
	p_mbr.xmin = p1x;
	p_mbr.ymax = p1y;
	p_mbr.ymin = p1y;
	if (!MBR::CheckOverlap(p_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int j = 0;

	int obj2PartIndex = 0;

	for (j = 0; j < comPoly->m_numPoints - 1 && ret != 1; j++)
	{
		double p3x = comPoly->m_pPoints[j].x;
		double p3y = comPoly->m_pPoints[j].y;
		double p4x = comPoly->m_pPoints[j + 1].x;
		double p4y = comPoly->m_pPoints[j + 1].y;

		if (p3x > p4x)
		{
			swap(&p3x, &p4x);
			swap(&p3y, &p4y);
		}

		r341 = GeometricFuc::ccw(p3x, p3y, p4x, p4y, p1x, p1y);

		if (r341 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
		{
			if ((p3x == p1x && p3y == p1y)
				|| (p4x == p1x && p4y == p1y))
			{
				ret = 1;
			}
			else if (p3x <= p1x && p1x <= p4x && min(p3y, p4y) <= p1y && max(p3y, p4y) >= p1y)
			{
				ret = 1;
			}
		}
	}

	return ret;
}
int SGeometricFuc::overlap(SPoint *objPoint, SCurve *comPoly, bool applyOption)
{

	if (applyOption)
	{
		return -1;
	}

	unsigned i = 0;
	int ret = -1;

	if (overlap(objPoint->m_point.x, objPoint->m_point.y, comPoly) == 1)
	{
		ret = 1;
	}

	return ret;
}


int SGeometricFuc::inside(SPoint *point, SSurface *poly, bool applyOption)
{

	if (applyOption)
	{
		return -1;
	}

	unsigned i = 0, ret = 1;

	if (inside(point->m_point.x, point->m_point.y, poly) == -1)
	{
		ret = -1;
	}
	return ret;
}
//  -1 : outside
//  1 : inside
int SGeometricFuc::inside(double x, double y, SSurface *poly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}

	if (poly == NULL)
		return -1;
	int i;
	int count = 0;
	int ic;
	count = 0;

	int lastExteriorIdx = 0;
	if (poly->m_numParts > 1)
	{
		lastExteriorIdx = poly->m_pParts[1] - 1;
	}
	else
	{
		lastExteriorIdx = poly->m_numPoints - 1;
	}

	int obj2PartIndex = 0;

	int entryIc = -1;


	//
	int *icList = NULL;
	static int ticList[100000];
	if (poly->m_numPoints > 100000)
	{
		int *ticList = new int[poly->m_numPoints];
		icList = ticList;
	}
	else
	{
		icList = ticList;
	}


	int listIndex = 0;
	for (i = 0; i < poly->m_numPoints - 1; i++)
	{
		if (poly->m_numParts > obj2PartIndex + 1)
		{
			if (poly->m_pParts[obj2PartIndex + 1] == i + 1)
			{
				obj2PartIndex++;
				continue;
			}
		}

		double max_px = max(max(poly->m_pPoints[i].x, poly->m_pPoints[i + 1].x) + 1, x + 1);

		ic = GeometricFuc::intersect_ccw(x, y, max_px, y, poly->m_pPoints[i].x, poly->m_pPoints[i].y, poly->m_pPoints[i + 1].x, poly->m_pPoints[i + 1].y);
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
		else if (ic == 10) // counterclockwise (in/out)
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
		else if (ic == 11) // clockwise (in/out)
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
double SGeometricFuc::GetAngle(POINT _p1, POINT _p2)
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
double SGeometricFuc::GetAngle(double p1x, double p1y, double p2x, double p2y)
{

	double x = p2x - p1x;
	double y = p2y - p1y;

	double d = sqrt(((double)(x)* x) + (y * y));
	double temp = x / d;

	if (y < 0)
	{
		return (acos(-1.0) * 2) - acos(temp);
	}

	return acos(temp);
}