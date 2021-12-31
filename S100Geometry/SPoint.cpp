#include "stdafx.h"
#include "SPoint.h"

SPoint::SPoint()
{
	type = 1;
}

SPoint::SPoint(double _x, double _y)
{
	type = 1;
	m_point.SetPoint(_x, _y);
}

SPoint::~SPoint()
{

}

void SPoint::SetPoint(double _x, double _y)
{
	m_point.SetPoint(_x, _y);
}