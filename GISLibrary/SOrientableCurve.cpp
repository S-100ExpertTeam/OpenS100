#include "stdafx.h"
#include "SOrientableCurve.h"

SOrientableCurve::SOrientableCurve()
{

}

SOrientableCurve::SOrientableCurve(const SOrientableCurve& other) : SGeometry(other)
{
	orientation = other.orientation;
}

SOrientableCurve::~SOrientableCurve()
{

}

SOrientableCurve SOrientableCurve::operator=(const SOrientableCurve& other)
{
	SGeometry::operator=(other);

	orientation = other.orientation;

	return *this;
}

