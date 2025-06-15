#include "stdafx.h"
#include "SCompositeCurve.h"
#include "SCurve.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace bg = boost::geometry;

SCompositeCurve::SCompositeCurve()
{
	
}

SCompositeCurve::SCompositeCurve(const SCompositeCurve& other)
	: SAbstractCurve(other)
{
	for (const auto& iter : m_listCurveLink)
	{
		SAbstractCurve* curve = new SAbstractCurve(*iter);
		AddCurve(curve);
	}
}

SCompositeCurve::~SCompositeCurve()
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++) {
		delete (*i);
		*i = nullptr;
	}

	m_listCurveLink.clear();
}

SCompositeCurve SCompositeCurve::operator=(const SCompositeCurve& other)
{
	//Geometry
	id = other.id;

	m_mbr.xmin = other.m_mbr.xmin;
	m_mbr.ymin = other.m_mbr.ymin;
	m_mbr.xmax = other.m_mbr.xmax;
	m_mbr.ymax = other.m_mbr.xmax;

	//SGeometry
	if (other.sizeOfPoint > 0)
	{
		sizeOfPoint = other.sizeOfPoint;
		viewPoints = new POINT[sizeOfPoint];
		memset(viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(viewPoints, other.viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	//SCompositeCurve
	for (const auto& iter : m_listCurveLink)
	{
		SAbstractCurve* curve = new SAbstractCurve(*iter);
		AddCurve(curve);
	}

	return *this;
}

SGeometryType SCompositeCurve::GetType()
{
	return SGeometryType::CompositeCurve;
}

void SCompositeCurve::SetMBR()
{
	m_mbr.InitMBR();

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		(*i)->SetMBR();
		m_mbr.CalcMBR((*i)->GetMBR());
	}
}

void SCompositeCurve::CreateD2Geometry(ID2D1Factory1* factory)
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		(*i)->CreateD2Geometry(factory);
	}
}

bool SCompositeCurve::ImportFromWkb(unsigned char* value, int size)
{
	Release();

	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type == (int)WKBGeometryType::wkbLineString)
	{
		auto curve = new SCurve();
		curve->ImportFromWkb(value, size);
		AddCurve(curve);
	}
	else if (type == (int)WKBGeometryType::wkbMultiLineString)
	{
		int numLineStrings = 0;

		memcpy_s(&numLineStrings, 4, value + 5, 4);

		int offset = 0;
		for (int i = 0; i < numLineStrings; i++)
		{
			auto curve = new SCurve();
			curve->ImportFromWkb(value + 9 + offset, size - 9 - offset);
			offset += curve->WkbSize();
			AddCurve(curve);
		}

		SetMBR();
	}
	else
	{
		return false;
	}

	return true;
}

bool SCompositeCurve::ExportToWkb(unsigned char** value, int* size)
{
	*size = WkbSize();
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbLineString;

	memcpy_s((*value) + 1, 4, &type, 4);

	int numPoints = GetPointCount();
	memcpy_s((*value) + 5, 4, &numPoints, 4);

	for (int i = 0; i < numPoints; i++)
	{
		double localX = 0;
		double localY = 0;

		auto p = GetXY(i);

		inverseProjection(p.x, p.y);

		memcpy_s((*value) + (9 + (16 * i)), 8, &p.x, 8);
		memcpy_s((*value) + (17 + (16 * i)), 8, &p.y, 8);
	}

	return true;
}

bool SCompositeCurve::ExportToWkbMultiLineString(unsigned char** value, int* size)
{
	*size = WkbSize();
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbMultiLineString;

	memcpy_s((*value) + 1, 4, &type, 4);

	int numLineStrings = (int)m_listCurveLink.size();
	memcpy_s((*value) + 5, 4, &numLineStrings, 4);

	int offset = 0;
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		int size = 0;
		auto mem = (*value) + (9 + offset);
		(*i)->ExportToWkb(&mem, &size);
		offset += size;
	}

	return true;
}

int SCompositeCurve::WkbSize()
{
	int size = 9;
	size += (GetPointCount() * 16);
	return size;
}

int SCompositeCurve::WkbSizeMultiLineString()
{
	int size = 9;
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		size += (*i)->WkbSize();
	}
	return size;
}

void SCompositeCurve::AddCurve(SAbstractCurve* curve)
{
	if (curve->GetType() == SGeometryType::Curve ||
		curve->GetType() == SGeometryType::CompositeCurve) {
		m_listCurveLink.push_back(curve);
	}
}

void SCompositeCurve::Release()
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		if ((*i)->GetType() == SGeometryType::Curve) {
			auto curve = (SCurve*)(*i);
			SafeRelease(&curve->pGeometry);
		}

		delete (*i);
	}

	m_listCurveLink.clear();
}

int SCompositeCurve::GetPointCount()
{
	return getNumPoint();
}

int SCompositeCurve::getNumPoint() const
{
	int count = 0;

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		count += (*i)->getNumPoint();
	}

	return count;
}

GeoPoint SCompositeCurve::GetXY(int index)
{
	GeoPoint result;
	int pointCount = 0;

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		int currentPointCount = (*i)->getNumPoint();
		if (index < pointCount + currentPointCount)
		{
			int localIndex = index - pointCount;
			result.x = (*i)->GetX(localIndex);
			result.y = (*i)->GetY(localIndex);
			return result;
		}

		pointCount += currentPointCount;
	}

	return result;
}

void SCompositeCurve::Set(int index, double x, double y)
{
	int pointCount = 0;

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		int currentPointCount = (*i)->getNumPoint();
		if (index < pointCount + currentPointCount)
		{
			int localIndex = index - pointCount;
			(*i)->Set(localIndex, x, y);
			return;
		}

		pointCount += currentPointCount;
	}
}

double SCompositeCurve::GetX()
{
	return getCenterPoint().GetX();
}

double SCompositeCurve::GetY()
{
	return getCenterPoint().GetY();
}

double SCompositeCurve::GetX(int index)
{
	return GetXY(index).GetX();
}

double SCompositeCurve::GetY(int index)
{
	return GetXY(index).GetY();
}

int SCompositeCurve::GetCurveCount() const
{
	return (int)m_listCurveLink.size();
}

SAbstractCurve* SCompositeCurve::GetCurve(int index)
{
	if (index >= 0 && index < GetCurveCount())
	{
		auto i = m_listCurveLink.begin();
		std::advance(i, index);
		return *i;
	}

	return nullptr;
}

std::vector<SAbstractCurve*> SCompositeCurve::GetCurveList()
{
	return m_listCurveLink;
}

SCurve* SCompositeCurve::GetCurveByRCID(int rcid)
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++) {
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

void SCompositeCurve::GetCurveList(std::list<SCurve*>& list)
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++) {
		if ((*i)->GetType() == SGeometryType::Curve) {
			list.push_back((SCurve*)(*i));
		}
		else if ((*i)->GetType() == SGeometryType::CompositeCurve) {
			auto compositeCurve = (SCompositeCurve*)(*i);
			compositeCurve->GetCurveList(list);
		}
	}
}

void SCompositeCurve::setSuppress(bool value)
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++) {
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

void SCompositeCurve::setCenterPoint()
{
	// allocate centerPoint for all sub curves
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		(*i)->setCenterPoint();
	}

	// allocate centerPoint for this composite curve
	if (centerPoint == nullptr)
	{
		centerPoint = std::make_unique<GeoPoint>();

		// Define Boost.Geometry types for 2D point and linestring
		typedef bg::model::d2::point_xy<double> Point;
		typedef bg::model::linestring<Point> LineString;

		// Convert all curve points to a Boost.Geometry linestring (after inverse projection)
		LineString line;
		for (int i = 0; i < getNumPoint(); i++)
		{
			double x = GetX(i);
			double y = GetY(i);
			inverseProjection(x, y);
			line.push_back(Point(x, y));
		}

		// Calculate the total length of the curve
		double total_length = bg::length(line);

		// Find the distance at the midpoint of the curve
		double mid_distance = total_length / 2.0;

		// Interpolate the midpoint along the curve
		Point midpoint;
		bg::line_interpolate(line, mid_distance, midpoint);

		double x = midpoint.x();
		double y = midpoint.y();

		// Project the midpoint back to the original coordinate system
		projection(x, y);
		if (nullptr == centerPoint)
		{
			centerPoint = std::make_unique<GeoPoint>();
		}

		// Set the centerPoint to the calculated midpoint
		centerPoint->SetPoint(x, y);
	}
}

std::list<GeoPoint> SCompositeCurve::getAllCenterPoint()
{
	std::list<GeoPoint> result;

	std::list<SCurve*> curveList;
	GetCurveList(curveList);

	for (auto i = curveList.begin(); i != curveList.end(); i++)
	{
		result.push_back((*i)->getCenterPoint());
	}

	return result;
}

GeoPoint SCompositeCurve::getCenterPoint()
{
	if (centerPoint == nullptr)
	{
		setCenterPoint();
	}

	return *centerPoint;
}