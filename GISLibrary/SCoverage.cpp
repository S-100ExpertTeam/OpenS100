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

SCoverage SCoverage::operator=(const SCoverage& other)
{
	if (m_pBoundary)
	{
		delete m_pBoundary;
		m_pBoundary = nullptr;
	}

	SGeometry::operator=(other);

	m_pBoundary = new MBR(other.m_mbr);
	m_xIndex = other.m_xIndex;
	m_yIndex = other.m_yIndex;

	return *this;
}

SGeometryType SCoverage::GetType()
{
	return SGeometryType::Coverage;
}



