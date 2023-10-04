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

SAbstractCurve* SAbstractCurve::clone()
{
	return new SAbstractCurve(*this);
}