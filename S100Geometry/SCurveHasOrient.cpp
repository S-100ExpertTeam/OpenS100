#include "stdafx.h"
#include "SCurveHasOrient.h"

SCurveHasOrient::SCurveHasOrient()
{

}

SCurveHasOrient::SCurveHasOrient(int rcid)
{
	this->m_id = rcid;
}

SCurveHasOrient::SCurveHasOrient(bool masking)
{
	this->m_masking = masking;
}

SCurveHasOrient::~SCurveHasOrient()
{
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