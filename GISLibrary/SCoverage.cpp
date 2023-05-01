#include "stdafx.h"
#include "SCoverage.h"

SCoverage::SCoverage()
{

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