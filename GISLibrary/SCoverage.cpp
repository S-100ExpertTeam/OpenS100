#include "stdafx.h"
#include "SCoverage.h"

SCoverage::SCoverage()
{

}

SCoverage::SCoverage(const SCoverage& other)
	: SGeometry(other)
{
	m_pBoundary = new MBR(other.m_mbr);
	m_xIndex = other.m_xIndex;
	m_yIndex = other.m_yIndex;
}

SCoverage::~SCoverage()
{
	delete m_pBoundary;
	m_pBoundary = nullptr;
}

SGeometryType SCoverage::GetType()
{
	return SGeometryType::Coverage;
}


SCoverage* SCoverage::Clone() const
{
	SCoverage* cv = new SCoverage();

	//Geometry
	cv->id = id;

	cv->m_mbr.xmin = m_mbr.xmin;
	cv->m_mbr.ymin = m_mbr.ymin;
	cv->m_mbr.xmax = m_mbr.xmax;
	cv->m_mbr.ymax = m_mbr.xmax;

	//SGeometry
	if (sizeOfPoint > 0)
	{
		if (cv->viewPoints)
		{
			delete[] cv->viewPoints;
			cv->viewPoints = nullptr;
		}

		cv->sizeOfPoint = sizeOfPoint;
		cv->viewPoints = new POINT[sizeOfPoint];
		memset(cv->viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(cv->viewPoints, viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	//SCoverage
	if (m_pBoundary)
		cv->m_pBoundary = new MBR(m_pBoundary->xmin, m_pBoundary->ymin, m_pBoundary->xmax, m_pBoundary->ymax);
	cv->m_xIndex = m_xIndex;
	cv->m_yIndex = m_yIndex;

	return cv;
}

