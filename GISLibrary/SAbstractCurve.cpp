#include "stdafx.h"
#include "SAbstractCurve.h"

SAbstractCurve::SAbstractCurve()
{

}

SAbstractCurve::~SAbstractCurve()
{

}

void SAbstractCurve::Set(int index, double x, double y)
{

}

SAbstractCurve* SAbstractCurve::Clone() const
{
	SAbstractCurve* ac = new SAbstractCurve();

	//Geometry
	ac->id = id;

	ac->m_mbr.xmin = m_mbr.xmin;
	ac->m_mbr.ymin = m_mbr.ymin;
	ac->m_mbr.xmax = m_mbr.xmax;
	ac->m_mbr.ymax = m_mbr.xmax;

	//SGeometry
	if (sizeOfPoint > 0)
	{
		if (ac->viewPoints)
		{
			delete[] ac->viewPoints;
			ac->viewPoints = nullptr;
		}

		ac->sizeOfPoint = sizeOfPoint;
		ac->viewPoints = new POINT[sizeOfPoint];
		memset(ac->viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(ac->viewPoints, viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	return ac;
}