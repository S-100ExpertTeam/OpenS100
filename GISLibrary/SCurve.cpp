#include "stdafx.h"
#include "SCurve.h"
#include "SPoint.h"
#include "SPoint.h"

#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../LatLonUtility/cpp_util.h"

#include <algorithm>
#include <sstream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace bg = boost::geometry;

SCurve::SCurve() 
{
	
}

SCurve::SCurve(const SCurve& other) : SAbstractCurve(other)
{
	m_masking = other.m_masking;
	suppress = other.suppress;
	m_firstPointID = other.m_firstPointID;
	m_lastPointID = other.m_lastPointID;

	m_numPoints = other.m_numPoints;
	if (m_numPoints > 0)
	{
		m_pPoints = new SVertex[m_numPoints];
		std::copy(other.m_pPoints, other.m_pPoints + m_numPoints, m_pPoints);
	}

	if (other.centerPoint)
	{
		centerPoint = new GeoPoint(*other.centerPoint);
	}
}

SCurve::~SCurve()
{
	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = nullptr;
	}

	if (centerPoint)
	{
		delete centerPoint;
		centerPoint = nullptr;
	}

	ResetRenderAttachment();
}

SCurve SCurve::operator=(const SCurve& other)
{
	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = nullptr;
	}

	if (centerPoint)
	{
		delete centerPoint;
		centerPoint = nullptr;
	}

	ResetRenderAttachment();

	SAbstractCurve::operator=(other);

	m_masking = other.m_masking;
	suppress = other.suppress;
	m_firstPointID = other.m_firstPointID;
	m_lastPointID = other.m_lastPointID;
	m_firstPoint.reset();
	m_lastPoint.reset();

	m_numPoints = other.m_numPoints;
	if (m_numPoints > 0)
	{
		m_pPoints = new SVertex[m_numPoints];
		std::copy(other.m_pPoints, other.m_pPoints + m_numPoints, m_pPoints);
	}

	if (other.centerPoint)
	{
		centerPoint = new GeoPoint(*other.centerPoint);
	}

	return *this;
}

SGeometryType SCurve::GetType()
{
	return SGeometryType::Curve;
}

int SCurve::GetRCID()
{
	// Asked for every frame, of every curve drawn: the text is read as a number once, and again
	// when the identifier is another.
	if (!m_rcidRead || m_rcidText != id)
	{
		m_rcid = cpp_util::stoi(id);
		m_rcidText = id;
		m_rcidRead = true;
	}
	return m_rcid;
}

int SCurve::GetNumPoints() const
{
	return m_numPoints;
}

int SCurve::getNumPoint() const
{
	return GetNumPoints();
}

double SCurve::GetX(int i) 
{
	if (m_pPoints == nullptr)
	{
		return 0;
	}

	return (m_pPoints + i)->x;
}

double SCurve::GetY(int i)
{
	if (m_pPoints == nullptr)
	{
		return 0;
	}
	return (m_pPoints + i)->y;
}

#pragma warning(disable:4244)
bool SCurve::ImportFromWkb(unsigned char* value, int /*size*/)
{
	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy(&type, value + 1, 4);

	if (type != (int)WKBGeometryType::wkbLineString)
	{
		return false;
	}

	memcpy(&m_numPoints, value + 5, 4);

	Init(m_numPoints);

	for (int i = 0; i < m_numPoints; i++)
	{
		memcpy(&m_pPoints[i].x, value + (9 + (16 * i)), 8);
		memcpy(&m_pPoints[i].y, value + (17 + (16 * i)), 8);

		projection(m_pPoints[i].x, m_pPoints[i].y);
	}

	SetMBR();

	return true;
}

bool SCurve::ExportToWkb(unsigned char** value, int* size)
{
	*size = 9 + (16 * m_numPoints);
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbLineString;

	memcpy((*value) + 1, &type, 4);

	memcpy((*value) + 5, &m_numPoints, 4);

	for (int i = 0; i < m_numPoints; i++)
	{
		double localX = m_pPoints[i].x;
		double localY = m_pPoints[i].y;
		inverseProjection(localX, localY);

		memcpy((*value) + (9 + (16 * i)), &localX, 8);
		memcpy((*value) + (17 + (16 * i)), &localY, 8);
	}

	return true;
}

int SCurve::WkbSize()
{
	return 9 + (16 * m_numPoints);
}

void SCurve::Init(int size)
{
	ResetRenderAttachment();

	SetID(0);

	delete[] m_pPoints;
	m_pPoints = new SVertex[size];
	m_numPoints = size;

	m_firstPointID = noPointID;
	m_lastPointID = noPointID;
	m_firstPoint.reset();
	m_lastPoint.reset();
}

void SCurve::Set(int index, double x, double y)
{
	m_pPoints[index].SetPoint(x, y);
}

void SCurve::SetMBR()
{
	m_mbr.InitMBR();

	for (int i = 0; i < m_numPoints; i++)
	{
		m_mbr.CalcMBR(m_pPoints[i].x, m_pPoints[i].y);
	}
}

void SCurve::Projection()
{
	// About a pole the segments, rhumb lines, are not straight on the chart: long ones get
	// points between their ends (RhumbLinePointsBetween). Few curves have such a segment, and
	// the others are left as they are.
	if (IsPolarProjection() && m_numPoints > 1)
	{
		struct Position
		{
			double x;
			double y;
		};
		std::vector<Position> positions;
		for (int i = 0; i + 1 < m_numPoints; i++)
		{
			const auto between = RhumbLinePointsBetween(m_pPoints[i].x, m_pPoints[i].y, m_pPoints[i + 1].x, m_pPoints[i + 1].y);
			if (between.empty() && positions.empty())
			{
				continue;
			}
			if (positions.empty())
			{
				for (int j = 0; j < i; j++)
				{
					positions.push_back({ m_pPoints[j].x, m_pPoints[j].y });
				}
			}
			positions.push_back({ m_pPoints[i].x, m_pPoints[i].y });
			for (const auto& [longitude, latitude] : between)
			{
				positions.push_back({ longitude, latitude });
			}
		}
		if (!positions.empty())
		{
			const SVertex& last = m_pPoints[m_numPoints - 1];
			positions.push_back({ last.x, last.y });
			// The ends stay the ends: the identifiers of their points stay with them.
			SVertex* points = new SVertex[positions.size()];
			for (std::size_t i = 0; i < positions.size(); i++)
			{
				points[i].SetPoint(positions[i].x, positions[i].y);
			}
			delete[] m_pPoints;
			m_pPoints = points;
			m_numPoints = static_cast<int>(positions.size());
		}
	}

	for (int i = 0; i < m_numPoints; i++)
	{
		projection(m_pPoints[i].x, m_pPoints[i].y);
	}
}

void SCurve::SetMultiplicationFactor(int comfX, int comfY)
{
	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].x /= comfX;
		m_pPoints[i].y /= comfY;
	}
}

void SCurve::SetRCID(int value)
{
	SetID(value);
}

bool SCurve::IsClosed()
{
	if (GetNumPoints() >= 3)
	{
		auto firstPoint = GetFirstPoint();
		auto lastPoint = GetLastPoint();

		if (firstPoint && lastPoint)
		{
			if (*firstPoint == *lastPoint)
			{
				return true;
			}
		}
	}

	return false;
}

void SCurve::SetEndPointIDs(int first, int last)
{
	m_firstPointID = first;
	m_lastPointID = last;
}

SPoint* SCurve::EndPoint(std::unique_ptr<SPoint>& kept, int index, int pointID)
{
	if (!kept)
	{
		kept = std::make_unique<SPoint>();
	}
	kept->SetPoint(m_pPoints[index].x, m_pPoints[index].y);
	if (pointID != noPointID)
	{
		kept->SetID(pointID);
	}
	return kept.get();
}

SPoint* SCurve::GetFirstPoint()
{
	if (GetNumPoints() > 0)
	{
		return EndPoint(m_firstPoint, 0, m_firstPointID);
	}

	return nullptr;
}

SPoint* SCurve::GetLastPoint()
{
	int numPoints = GetNumPoints();
	if (numPoints > 0)
	{
		return EndPoint(m_lastPoint, numPoints - 1, m_lastPointID);
	}

	return nullptr;
}

double SCurve::GetX()
{
	return getCenterPoint().GetX();
}

double SCurve::GetY()
{
	return getCenterPoint().GetY();
}

std::string SCurve::ToString()
{
	std::stringstream ss;
	ss.precision(7);

	for (int i = 0; i < GetNumPoints(); i++)
	{
		if (i != 0)
		{
			ss << " ";
		}

		double x = GetX(i);
		double y = GetY(i);
		inverseProjection(x, y);
		ss << y << " " << x;
	}

	return ss.str();
}

void SCurve::SetMasking(bool value)
{
	m_masking = value;
}

bool SCurve::GetMasking()
{
	return m_masking;
}

void SCurve::SetSuppress(bool value)
{
	suppress = value;
}

bool SCurve::GetSuppress()
{
	return suppress;
}

bool SCurve::isDraw()
{
	if (GetSuppress() == false &&
		GetMasking() == false) {
		return true;
	}

	return false;
}

void SCurve::setCenterPoint()
{
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
		centerPoint = new GeoPoint();
	}
	
	// Set the centerPoint to the calculated midpoint
	centerPoint->SetPoint(x, y);
}

GeoPoint SCurve::getCenterPoint()
{
	if (nullptr == centerPoint)
	{
		setCenterPoint();
	}

	return *centerPoint;
}