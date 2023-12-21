#include "stdafx.h"
#include "SOrientableCurve.h"

SOrientableCurve::SOrientableCurve()
{

}

SOrientableCurve::~SOrientableCurve()
{

}

SOrientableCurve* SOrientableCurve::Clone() const
{
	SOrientableCurve* oc = new SOrientableCurve();

	//Geometry
	oc->id = id;

	oc->m_mbr.xmin = m_mbr.xmin;
	oc->m_mbr.ymin = m_mbr.ymin;
	oc->m_mbr.xmax = m_mbr.xmax;
	oc->m_mbr.ymax = m_mbr.xmax;

	//SGeometry
	if (sizeOfPoint > 0)
	{
		if (oc->viewPoints)
		{
			delete[] oc->viewPoints;
			oc->viewPoints = nullptr;
		}

		oc->sizeOfPoint = sizeOfPoint;
		oc->viewPoints = new POINT[sizeOfPoint];
		memset(oc->viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(oc->viewPoints, viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	return oc;
}


