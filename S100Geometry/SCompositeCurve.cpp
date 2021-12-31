#include "stdafx.h"
#include "SCompositeCurve.h"
#include "SCurve.h"

SCompositeCurve::SCompositeCurve()
{
	type = 2;
}

SCompositeCurve::~SCompositeCurve()
{

}

void SCompositeCurve::SetMBR()
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		m_mbr.CalcMBR((*i).GetCurve()->GetMBRRef());
	}
}

void SCompositeCurve::CreateD2Geometry(ID2D1Factory1* factory)
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		(*i).GetCurve()->CreateD2Geometry(factory);
	}
}


std::list<ID2D1PathGeometry*> SCompositeCurve::GetD2Geometry()
{
	std::list<ID2D1PathGeometry*> ret;
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		ret.push_back((*i).GetCurve()->GetD2Geometry());
	}
	return ret;
}

std::list<ID2D1PathGeometry*> SCompositeCurve::GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler)
{
	std::list<ID2D1PathGeometry*> ret;
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		ret.push_back((*i).GetCurve()->GetNewD2Geometry(factory, scaler));
	}
	return ret;
}