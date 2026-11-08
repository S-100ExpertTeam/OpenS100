#include "stdafx.h"
#include "GeoPolygon.h"
#include "GeoPoint.h"
#include "GeoCommonFuc.h"
#include "Scaler.h"

GeoPolygon::GeoPolygon(void)
{
	Parts = NULL;
	Points = NULL;
}

GeoPolygon::GeoPolygon(const GeoPolygon& other) : Geometry(other)
{
	NumPoints = other.NumPoints;
	if (NumPoints == 0)
		return;

	Points = new GeoPoint[NumPoints];
	for (int i = 0; i < NumPoints; i++)
	{
		Points[i].id = other.Points[i].id;

		m_mbr.xmin = other.m_mbr.xmin;
		m_mbr.ymin = other.m_mbr.ymin;
		m_mbr.xmax = other.m_mbr.xmax;
		m_mbr.ymax = other.m_mbr.ymax;

		Points[i].x = other.Points[i].x;
		Points[i].y = other.Points[i].y;

	}

	NumParts = other.NumParts;
	if (NumParts == 0)
		return;

	Parts = new int[NumParts];
	for (int i = 0; i < NumParts; i++)
	{
		Parts[i] = other.Parts[i];
	}
}

GeoPolygon::~GeoPolygon(void)
{
	if (Parts != NULL)
		delete[] Parts;
	if (Points != NULL)
		delete[] Points;
}

GeoPolygon GeoPolygon::operator=(const GeoPolygon& other)
{
	if (Parts)
	{
		delete[] Parts;
		Parts = nullptr;
	}
		
	if (Points)
	{
		delete[] Points;
		Points = nullptr;
	}
		

	Geometry::operator=(other);

	NumPoints = other.NumPoints;
	if (NumPoints == 0)
		return *this;

	Points = new GeoPoint[NumPoints];
	for (int i = 0; i < NumPoints; i++)
	{
		Points[i].id = other.Points[i].id;

		m_mbr.xmin = other.m_mbr.xmin;
		m_mbr.ymin = other.m_mbr.ymin;
		m_mbr.xmax = other.m_mbr.xmax;
		m_mbr.ymax = other.m_mbr.ymax;

		Points[i].x = other.Points[i].x;
		Points[i].y = other.Points[i].y;

	}

	NumParts = other.NumParts;
	if (NumParts == 0)
		return *this;

	Parts = new int[NumParts];
	for (int i = 0; i < NumParts; i++)
	{
		Parts[i] = other.Parts[i];
	}

	return *this;
}

