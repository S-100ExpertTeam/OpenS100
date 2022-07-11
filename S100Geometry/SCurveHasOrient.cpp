#include "stdafx.h"
#include "SCurveHasOrient.h"

//SCurveHasOrient::SCurveHasOrient(bool _parentOrient, SCurve* _curve)
//{
//	parentOrient = _parentOrient;
//	curve = _curve;
//}
//
//SCurveHasOrient::SCurveHasOrient(int _parentOrient, SCurve* _curve)
//{
//	if (1 == _parentOrient)
//	{
//		parentOrient = true;
//	}
//	else if (2 == _parentOrient)
//	{
//		parentOrient = false;
//	}
//	curve = _curve;
//}
//
//SCurveHasOrient::SCurveHasOrient(SCurve* curve, bool parentOrient, bool masking, bool isDuplicated)
//{
//	this->curve = curve;
//	this->parentOrient = parentOrient;
//	this->m_masking = masking;
//	this->m_isDuplicated = isDuplicated;
//}

SCurveHasOrient::SCurveHasOrient()
{

}

SCurveHasOrient::SCurveHasOrient(bool masking)
{
	//this->curve = curve;
	this->m_masking = masking;
}

//SCurveHasOrient::SCurveHasOrient(SCurve* curve, bool masking)
//{
//	//this->curve = curve;
//	this->m_masking = masking;
//}

SCurveHasOrient::~SCurveHasOrient()
{
}

//bool SCurveHasOrient::GetParentOrient()
//{
//	return parentOrient;
//}

//SCurve* SCurveHasOrient::GetCurve()
//{
//	return curve;
//}

void SCurveHasOrient::SetMasking(bool value)
{
	m_masking = value;
}

bool SCurveHasOrient::GetMasking()
{
	return m_masking;
}

//void SCurveHasOrient::SetIsDuplicated(bool value)
//{
//	m_isDuplicated = value;
//}
//
//bool SCurveHasOrient::GetIsDuplicated()
//{
//	return m_isDuplicated;
//}

void SCurveHasOrient::SetSuppress(bool value)
{
	suppress = value;
}

bool SCurveHasOrient::GetSuppress()
{
	return suppress;
}