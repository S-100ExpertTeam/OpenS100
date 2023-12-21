#include "stdafx.h"
#include "GeoMultiPointZ.h"
#include "GeoPointZ.h"
#include "Scaler.h"

GeoMultiPointZ::GeoMultiPointZ()
{

}

GeoMultiPointZ::GeoMultiPointZ(int size)
{
	m_numPoints = size;
	m_pPoints = new GeoPointZ[size];
}

GeoMultiPointZ::GeoMultiPointZ(double _x, double _y, double _z)
{
	m_numPoints = 1;
	m_pPoints = new GeoPointZ[m_numPoints];
	m_pPoints[0].SetPoint(_x, _y, _z);
}

GeoMultiPointZ::GeoMultiPointZ(int size, double* _x, double* _y, double* _z)
{
	m_numPoints = size;
	m_pPoints = new GeoPointZ[m_numPoints];

	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].SetPoint(_x[i], _y[i], _z[i]);
	}
}

GeoMultiPointZ::~GeoMultiPointZ()
{
	delete [] m_pPoints;
	m_pPoints = nullptr;
}

void GeoMultiPointZ::SetPoint(int index, double _x, double _y, double _z)
{
	m_pPoints[index].SetPoint(_x, _y, _z);
}

#pragma warning(disable:4018)
void GeoMultiPointZ::DrawGeometry(HDC &hDC, Scaler *scaler, double offset)
{
	POINT point;
	
	HBRUSH m_hBrush = (HBRUSH)GetStockObject(RGB(165, 191, 221));

	for(unsigned i = 0; i< m_numPoints ; i++)
	{
		scaler->WorldToDevice(m_pPoints[i].x + offset, m_pPoints[i].y, &(point.x), &(point.y));
		RECT r;
		r.left = point.x - 2;
		r.top = point.y - 2;
		r.right = point.x + 2;
		r.bottom = point.y + 2;
		
		::FillRect(hDC, &r, m_hBrush);
	}
}

GeoMultiPointZ* GeoMultiPointZ::Clone() const
{
	GeoMultiPointZ* mpt = new GeoMultiPointZ();

	mpt->m_numPoints = m_numPoints;
	if (m_numPoints == 0)
		return mpt;

	mpt->m_pPoints = new GeoPointZ[m_numPoints];
	for (int i = 0; i < m_numPoints; i++)
	{
		mpt->m_pPoints[i].x = m_pPoints[i].x;
		mpt->m_pPoints[i].y = m_pPoints[i].y;
		mpt->m_pPoints[i].z = m_pPoints[i].z;
	}

	return mpt;
}
