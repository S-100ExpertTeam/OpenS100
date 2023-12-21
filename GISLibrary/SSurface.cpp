#include "stdafx.h"
#include "SSurface.h"
#include "SCommonFuction.h"
#include "GISLibrary.h"

#include "../GeoMetryLibrary/MBR.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"

#include <algorithm>

SSurface::SSurface()
{
	
}

SSurface::SSurface(MBR* mbr)
{
	m_numParts = 1;
	m_numPoints = 5;
	m_pParts = new int[1];
	m_pParts[0] = 0;
	m_pPoints = new GeoPoint[5];
	m_pPoints[0].x = mbr->xmin;
	m_pPoints[0].y = mbr->ymin;
	m_pPoints[1].x = mbr->xmax;
	m_pPoints[1].y = mbr->ymin;
	m_pPoints[2].x = mbr->xmax;
	m_pPoints[2].y = mbr->ymax;
	m_pPoints[3].x = mbr->xmin;
	m_pPoints[3].y = mbr->ymax;
	m_pPoints[4].x = mbr->xmin;
	m_pPoints[4].y = mbr->ymin;
	m_mbr.xmin = mbr->xmin;
	m_mbr.ymin = mbr->ymin;
	m_mbr.xmax = mbr->xmax;
	m_mbr.ymax = mbr->ymax;
}

SSurface::SSurface(std::vector<POINT>& points, std::vector<int>& parts)
{
	Set(points, parts);
}

SSurface::~SSurface()
{
	delete[] m_pPoints;
	m_pPoints = nullptr;

	delete[] m_pParts;
	m_pParts = nullptr;

	delete m_centerPoint;
	m_centerPoint = nullptr;

	SafeRelease(&pGeometry);

	for (auto i = curveList.begin(); i != curveList.end(); i++) {
		delete (*i);
		*i = nullptr;
	}

	curveList.clear();
}

SGeometryType SSurface::GetType()
{
	return SGeometryType::Surface;
}

#pragma warning(disable:4244)
void SSurface::CreateD2Geometry(ID2D1Factory1* factory)
{
	if (nullptr == pGeometry &&
		m_numPoints > 2)
	{
		auto points = new D2D1_POINT_2F[m_numPoints];

		for (int i = 0; i < m_numPoints; i++)
		{
			points[i].x = m_pPoints[i].x;
			points[i].y = -m_pPoints[i].y;
		}

		if (m_numParts == 1)
		{
			auto result = factory->CreatePathGeometry((ID2D1PathGeometry **)&pGeometry);
			if (SUCCEEDED(result))
			{
				ID2D1PathGeometry *pPathGeometry = (ID2D1PathGeometry *)pGeometry;
				ID2D1GeometrySink *pSink = nullptr;
				result = pPathGeometry->Open(&pSink);
				if (SUCCEEDED(result))
				{
					pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
					pSink->AddLines(&points[1], m_numPoints - 1);
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
					result = pSink->Close();
					if (SUCCEEDED(result))
					{
						SafeRelease(&pSink);
					}
				}
			}
		}
		else if (m_numParts > 1)
		{
			// Area
			ID2D1PathGeometry **segGeometry = new ID2D1PathGeometry*[m_numParts];
			for (int i = 0; i < m_numParts; i++)
			{
				auto result = factory->CreatePathGeometry(&segGeometry[i]);
				if (SUCCEEDED(result))
				{
					ID2D1GeometrySink *pSink = nullptr;
					result = segGeometry[i]->Open(&pSink);
					if (SUCCEEDED(result))
					{
						pSink->BeginFigure(points[m_pParts[i]], D2D1_FIGURE_BEGIN_FILLED);
						pSink->AddLines(&points[m_pParts[i] + 1], GetNumPointPerPart(i) - 1);
						pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
						result = pSink->Close();
						if (SUCCEEDED(result))
						{
							SafeRelease(&pSink);
						}
					}

				}
			}

			auto result = factory->CreateGeometryGroup(
				D2D1_FILL_MODE_ALTERNATE,
				(ID2D1Geometry **)segGeometry,
				m_numParts,
				(ID2D1GeometryGroup **)&(pGeometry));

			for (int i = 0; i < m_numParts; i++)
			{
				SafeRelease(&segGeometry[i]);
			}

			delete[] segGeometry;
			segGeometry = nullptr;
		}

		delete[] points;
		points = nullptr;

		//for (auto i = compositeCurve->m_listCurveLink.begin(); i != compositeCurve->m_listCurveLink.end(); i++)
		//{
		//	(*i).GetCurve()->CreateD2Geometry(factory);
		//}

		for (auto i = curveList.begin(); i != curveList.end(); i++)
		{
			(*i)->CreateD2Geometry(factory);
		}
	}
}

int SSurface::GetNumPart()
{
	return m_numParts;
}

int SSurface::getNumPoint() const
{
	return m_numPoints;
}

int SSurface::getPart(int index)
{
	if (index >= 0 && index < GetNumPart()) {
		return m_pParts[index];
	}

	return -1;
}

int SSurface::getLastPointIndexOfExterior()
{
	int result = 0;
	if (GetNumPart() > 1)
	{
		result = getPart(1) - 1;
	}
	else
	{
		result = getNumPoint() - 1;
	}

	return result;
}

int SSurface::GetNumPointPerPart(int partIndex)
{
	if (partIndex < 0 || partIndex >= m_numParts)
	{
		return 0;
	}

	if (partIndex == m_numParts - 1)
	{
		return m_numPoints - m_pParts[partIndex];
	}

	return m_pParts[partIndex + 1] - m_pParts[partIndex];
}

ID2D1PathGeometry* SSurface::GetD2Geometry()
{
	return pGeometry;
}

void SSurface::CalculateCenterPoint()
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	double x1, y1, x2, y2, tmpArea;

	for (int i = 0; i < m_numPoints; i++)
	{
		j = (i + 1) % m_numPoints;

		x1 = m_pPoints[i].x;
		y1 = m_pPoints[i].y;
		x2 = m_pPoints[j].x;
		y2 = m_pPoints[j].y;

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	if (!m_centerPoint)
	{
		m_centerPoint = new GeoPoint();
	}

	m_centerPoint->x = centerX;
	m_centerPoint->y = centerY;
}

ID2D1PathGeometry* SSurface::GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler)
{
	if (m_numPoints > 2)
	{
		ID2D1PathGeometry* newGeometry = nullptr;
		auto points = new D2D1_POINT_2F[m_numPoints];

		 //memory leak(little bit)
		for (int i = 0; i < m_numPoints; i++)
		{
			scaler->WorldToDevice_F(m_pPoints[i].x, m_pPoints[i].y, &points[i].x, &points[i].y);
		}

		if (m_numParts == 1)
		{
			//memory leak
			auto result = factory->CreatePathGeometry((ID2D1PathGeometry **)&newGeometry);
			if (SUCCEEDED(result))
			{
				ID2D1PathGeometry *pPathGeometry = (ID2D1PathGeometry *)newGeometry;
				ID2D1GeometrySink *pSink = nullptr;
				result = pPathGeometry->Open(&pSink);
				if (SUCCEEDED(result))
				{
					pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
					pSink->AddLines(&points[1], m_numPoints - 1);
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
					result = pSink->Close();
					if (SUCCEEDED(result))
					{
						SafeRelease(&pSink);
					}
				}
			}
		}
		else if (m_numParts > 1)
		{
			// Area
			ID2D1PathGeometry **segGeometry = new ID2D1PathGeometry*[m_numParts];
			for (int i = 0; i < m_numParts; i++)
			{
				//memory leak
				auto result = factory->CreatePathGeometry(&segGeometry[i]);
				if (SUCCEEDED(result))
				{
					ID2D1GeometrySink *pSink = nullptr;
					result = segGeometry[i]->Open(&pSink);
					if (SUCCEEDED(result))
					{
						pSink->BeginFigure(points[m_pParts[i]], D2D1_FIGURE_BEGIN_FILLED);
						pSink->AddLines(&points[m_pParts[i] + 1], GetNumPointPerPart(i) - 1);
						pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
						result = pSink->Close();
						if (SUCCEEDED(result))
						{
							SafeRelease(&pSink);
						}
					}

				}
			}

			auto result = factory->CreateGeometryGroup(
				D2D1_FILL_MODE_ALTERNATE,
				(ID2D1Geometry **)segGeometry,
				m_numParts,
				(ID2D1GeometryGroup **)&(newGeometry));

			for (int i = 0; i < m_numParts; i++)
			{
				SafeRelease(&segGeometry[i]);
			}

			delete[] segGeometry;
			segGeometry = nullptr;
	     }

		delete[] points;
		points = nullptr;

		return newGeometry;

	}
	return nullptr;
}

void SSurface::AddCurve(SAbstractCurve* curve)
{
	curveList.push_back(curve);
}

void SSurface::Init()
{
	m_numParts = 0;
	m_numPoints = 0;
	
	delete[] m_pParts; 
	m_pParts = nullptr;

	delete[] m_pPoints;
	m_pPoints = nullptr;
	
	delete m_centerPoint;
	m_centerPoint = nullptr;

	Release();
	//curveList.clear();

	SafeRelease(&pGeometry);
}

void SSurface::Set(std::vector<POINT>& points, std::vector<int>& parts)
{
	m_numParts = (int)parts.size();
	m_numPoints = (int)points.size();

	if (m_numParts < 2)
	{
		m_pParts = new int[1];
		m_pParts[0] = 0;
	}
	else
	{
		m_pParts = new int[m_numParts];
		m_pParts[0] = 0;
		int size = 0;
		size = (int)parts.size();
		for (int i = 1; i < size; i++)
		{
			m_pParts[i] = parts.at(i - 1);
		}
	}

	m_pPoints = new GeoPoint[m_numPoints];

	if (m_numPoints > SGeometry::sizeOfPoint)
	{
		delete[] SGeometry::viewPoints;
		SGeometry::viewPoints = nullptr;
	}

	if (SGeometry::viewPoints == nullptr)
	{
		SGeometry::sizeOfPoint = m_numPoints * 1.5;
		SGeometry::viewPoints = new CPoint[SGeometry::sizeOfPoint];
	}

	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].SetPoint(points[i].x / 10000000.0, points[i].y / 10000000.0);
		projection(m_pPoints[i].x, m_pPoints[i].y);
		m_mbr.CalcMBR(m_pPoints[i].x, m_pPoints[i].y);
	}
}

void SSurface::Release()
{
	for (auto i = curveList.begin(); i != curveList.end(); i++)
	{
		if ((*i)->GetType() == SGeometryType::Curve) {
			auto curve = (SCurve*)(*i);
			SafeRelease(&curve->pGeometry);
		}
		
		(*i)->Release();
		delete (*i);
	}
	curveList.clear();
}

bool SSurface::ImportFromWkb(unsigned char* value, int size)
{
	Init();

	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type != (int)WKBGeometryType::wkbPolygon)
	{
		return false;
	}

	memcpy_s(&m_numParts, 4, value + 5, 4);
	m_pParts = new int[m_numParts];
	memset(m_pParts, 0, sizeof(int) * m_numParts);

	int offset = 0;

	std::vector<GeoPoint> localPointArray;

	for (int i = 0; i < m_numParts; i++)
	{
		int numPointPerPart = 0;
		memcpy_s(&numPointPerPart, 4, value + 9 + offset, 4);
		offset += 4;

		m_pParts[i] = (int)localPointArray.size();

		auto curve = new SCurve();
		curve->Init(numPointPerPart);
		curveList.push_back(curve);

		for (int j = 0; j < numPointPerPart; j++)
		{
			GeoPoint point;
			memcpy_s(&point.x, 8, value + 9 + offset, 8);
			memcpy_s(&point.y, 8, value + 17 + offset, 8);
			offset += 16;

			projection(point.x, point.y);
			localPointArray.push_back(point);

			curve->Set(j, point.x, point.y);
		}
	}

	m_numPoints = (int)localPointArray.size();

	m_pPoints = new GeoPoint[m_numPoints];
	std::copy(localPointArray.begin(), localPointArray.end(), m_pPoints);

	CalculateCenterPoint();
	SetMBR();

	return true;
}

bool SSurface::ExportToWkb(unsigned char** value, int* size)
{
	*size = WkbSize();
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbPolygon;

	memcpy_s((*value) + 1, 4, &type, 4);

	int numRings = m_numParts;
	memcpy_s((*value) + 5, 4, &numRings, 4);

	auto mem = (*value) + 9;
	for (int i = 0; i < numRings; i++)
	{
		int numPoints = GetNumPointPerPart(i);
		memcpy_s(mem, 4, &numPoints, 4);
		mem += 4;

		for (int j = 0; j < numPoints; j++)
		{
			auto xy = GetXY(i, j);

			inverseProjection(xy.x, xy.y);

			memcpy_s(mem, 8, &xy.x, 8);
			mem += 8;

			memcpy_s(mem, 8, &xy.y, 8);
			mem += 8;
		}
	}

	return true;
}

int SSurface::WkbSize()
{
	int size = 9;
	
	for (int i = 0; i < m_numParts; i++)
	{
		size += (4 + (16 * GetNumPointPerPart(i)));
	}

	return size;
}

void SSurface::SetMBR()
{
	m_mbr.InitMBR();

	int outerRingPointCount = GetNumPointPerPart(0);
	for (int i = 0; i < outerRingPointCount; i++)
	{
		m_mbr.CalcMBR(m_pPoints[i].x, m_pPoints[i].y);
	}
}

GeoPoint SSurface::GetXY(int ringIndex, int pointIndex)
{
	if (ringIndex >= 0 && ringIndex < m_numParts)
	{
		int pointCountPerPart = GetNumPointPerPart(ringIndex);

		if (pointIndex >= 0 && pointIndex < pointCountPerPart)
		{
			return m_pPoints[m_pParts[ringIndex] + pointIndex];
		}
	}

	return GeoPoint();
}

void SSurface::SetXY(int ringIndex, int pointIndex, double x, double y)
{
	if (ringIndex >= 0 && ringIndex < m_numParts)
	{
		int pointCountPerPart = GetNumPointPerPart(ringIndex);

		if (pointIndex >= 0 && pointIndex < pointCountPerPart)
		{
			m_pPoints[m_pParts[ringIndex] + pointIndex].x = x;
			m_pPoints[m_pParts[ringIndex] + pointIndex].y = y;
		}
	}
}

double SSurface::GetX()
{
	return GetXY(0, 0).GetX();
}

double SSurface::GetY()
{
	return GetXY(0, 0).GetY();
}

double SSurface::GetX(int index)
{
	if (index < 0 || index >= getNumPoint()) {
		return 0;
	}

	return m_pPoints[index].GetX();
}

double SSurface::GetY(int index)
{
	if (index < 0 || index >= getNumPoint()) {
		return 0;
	}

	return m_pPoints[index].GetY();
}

int SSurface::GetRingCount() const
{
	return (int)curveList.size();
}

SAbstractCurve* SSurface::GetRing(int index) const
{
	if (index >= 0 && index < GetRingCount())
	{
		return curveList.at(index);
	}

	return nullptr;
}

//std::vector<SAbstractCurve*> SSurface::GetCurveList()
//{
//	return curveList;
//}

SCurve* SSurface::GetCurve(int rcid)
{
	for (auto i = curveList.begin(); i != curveList.end(); i++) {
		if ((*i)->GetType() == SGeometryType::Curve) {
			if ((*i)->GetIDAsInt() == rcid) {
				auto curve = (SCurve*)(*i);
				return curve;
			}
		}
		else if ((*i)->GetType() == SGeometryType::CompositeCurve) {
			auto compositeCurve = (SCompositeCurve*)(*i);
			return compositeCurve->GetCurveByRCID(rcid);
		}
	}

	return nullptr;
}

void SSurface::GetCurveList(std::list<SCurve*>& list)
{
	for (auto i = curveList.begin(); i != curveList.end(); i++) {
		if ((*i)->GetType() == SGeometryType::Curve) {
			auto curve = (SCurve*)(*i);
			list.push_back(curve);
		}
		else if ((*i)->GetType() == SGeometryType::CompositeCurve) {
			auto compositeCurve = (SCompositeCurve*)(*i);
			compositeCurve->GetCurveList(list);
		}
	}
}

void SSurface::setSuppress(bool value)
{
	for (auto i = curveList.begin(); i != curveList.end(); i++) {
		if ((*i)->GetType() == SGeometryType::Curve) {
			auto curve = (SCurve*)(*i);
			curve->SetSuppress(value);
		}
		else if ((*i)->GetType() == SGeometryType::CompositeCurve) {
			auto compositeCurve = (SCompositeCurve*)(*i);
			compositeCurve->setSuppress(value);
		}
	}
}

SSurface* SSurface::Clone() const
{
	SSurface* sf = new SSurface();

	//Geometry
	sf->id = id;

	sf->m_mbr.xmin = m_mbr.xmin;
	sf->m_mbr.ymin = m_mbr.ymin;
	sf->m_mbr.xmax = m_mbr.xmax;
	sf->m_mbr.ymax = m_mbr.xmax;

	//SGeometry
	if (sizeOfPoint > 0)
	{
		if (sf->viewPoints)
		{
			delete[] sf->viewPoints;
			sf->viewPoints = nullptr;
		}

		sf->sizeOfPoint = sizeOfPoint;
		sf->viewPoints = new POINT[sizeOfPoint];
		memset(sf->viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(sf->viewPoints, viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	//SSurface
	sf->m_numPoints = m_numPoints;
	if (m_numPoints == 0)
		return sf;

	sf->m_pPoints = new GeoPoint[m_numPoints];
	for (int i = 0; i < m_numPoints; i++)
	{
		sf->m_pPoints[i].id = m_pPoints[i].id;

		sf->m_mbr.xmin = m_mbr.xmin;
		sf->m_mbr.ymin = m_mbr.ymin;
		sf->m_mbr.xmax = m_mbr.xmax;
		sf->m_mbr.ymax = m_mbr.ymax;

		sf->m_pPoints[i].x = m_pPoints[i].x;
		sf->m_pPoints[i].y = m_pPoints[i].y;
	}

	for (const auto& iter : curveList)
	{
		sf->curveList.push_back(iter->Clone());
	}

	if (m_centerPoint)
		sf->m_centerPoint = m_centerPoint->Clone();

	sf->m_numParts = m_numParts;
	if (m_numParts == 0)
		return sf;
	sf->m_pParts = new int[m_numParts];
	for (int i = 0; i < m_numParts; i++)
	{
		sf->m_pParts[i] = m_pParts[i];
	}

	return sf;
}

