#include "stdafx.h"
#include "SAbstractCurve.h"

SAbstractCurve::SAbstractCurve()
{

}

SAbstractCurve::SAbstractCurve(const SAbstractCurve& other) :SGeometry(other)
{
	
}

SAbstractCurve::~SAbstractCurve()
{

}

SAbstractCurve SAbstractCurve::operator=(const SAbstractCurve& other)
{
	SGeometry::operator=(other);

	return *this;
}

void SAbstractCurve::Set(int index, double x, double y)
{

}
