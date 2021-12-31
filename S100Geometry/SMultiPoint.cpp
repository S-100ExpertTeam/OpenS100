#include "stdafx.h"
#include "SMultiPoint.h"

SMultiPoint::SMultiPoint()
{
	type = 4;
}

SMultiPoint::~SMultiPoint()
{
	delete pColor; pColor = nullptr;
	delete m_pPoints; m_pPoints = nullptr;
}

int SMultiPoint::GetNumPoints()
{
	return m_numPoints;
}

double SMultiPoint::GetX(int index)
{
	try
	{
		if (m_pPoints)
		{
			return m_pPoints->at(index).x;
		}
	}
	catch (std::out_of_range)
	{
		return 0;
	}
	return 0;
}

double SMultiPoint::GetY(int index)
{
	try
	{
		if (m_pPoints)
		{
			return m_pPoints->at(index).y;
		}
	}
	catch (std::out_of_range)
	{
		return 0;
	}

	return 0;
}