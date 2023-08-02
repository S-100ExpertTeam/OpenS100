#include "stdafx.h"
#include "SCurveHasOrient.h"

SCurveHasOrient::SCurveHasOrient()
{

}

SCurveHasOrient::~SCurveHasOrient()
{
}

SGeometryType SCurveHasOrient::GetType()
{
	return SGeometryType::CurveHasOrient;
}

void SCurveHasOrient::SetMasking(bool value)
{
	m_masking = value;
}

bool SCurveHasOrient::GetMasking()
{
	return m_masking;
}

void SCurveHasOrient::SetSuppress(bool value)
{
	suppress = value;
}

bool SCurveHasOrient::GetSuppress()
{
	return suppress;
}

bool SCurveHasOrient::isDraw()
{
	if (GetSuppress() == false &&
		GetMasking() == false) {
		return true;
	}

	return false;
}