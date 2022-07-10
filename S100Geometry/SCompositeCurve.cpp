#include "stdafx.h"
#include "SCompositeCurve.h"
#include "SCurve.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"

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

bool SCompositeCurve::ImportFromWkb(char* value, int size)
{
	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type != (int)WKBGeometryType::wkbMultiLineString)
	{
		return false;
	}

	int numLineStrings = 0;

	memcpy_s(&numLineStrings, 4, value + 5, 4);
	
	for (int i = 0; i < numLineStrings; i++)
	{
	//	SCurveHasOrient curve(1,)
	}

	return true;
}

bool SCompositeCurve::ExportToWkb(char** value, int* size)
{
	return false;
}

void SCompositeCurve::AddCurve(SCurve* curve, bool masking)
{
	m_listCurveLink.push_back(SCurveHasOrient(curve, masking));
}

void SCompositeCurve::Release()
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		delete (*i).GetCurve();
	}
}