#include "stdafx.h"
#include "Line.h"

namespace libS100Engine
{
	void Line::initialize()
	{
		type = line;
		strokeWidth = rotation = 0;
		strokeDasharray = L"";
		fill = false;
	}

	Line::Line()
	{
		initialize();
	}

	Line::Line(std::vector<std::wstring> points)
	{
		initialize();

		for (int i = 0; i < (int)points.size(); i += 2)
		{
			POINTF point;

			point.x = (FLOAT)_wtof(points[i].c_str());
			point.y = (FLOAT)_wtof(points[i + 1].c_str());
			m_points.push_back(point);
		}
	}

	Line::Line(POINTF points[], int pointsCount)
	{
		initialize();

		for (int i = 0; i < pointsCount; i++)
		{
			m_points.push_back(points[i]);
		}
	}

	Line::~Line()
	{

	}

	void Line::AddPoint(POINTF point)
	{
		m_points.push_back(point);
	}

	void Line::AddPoints(std::vector<std::wstring> points)
	{
		for (int i = 0; i < (int)points.size() && i + 1 <= (int)points.size() - 1; i += 2)
		{

			POINTF point;
			try
			{
				point.x = (FLOAT)_wtof(points[i].c_str());
				point.y = (FLOAT)_wtof(points[i + 1].c_str());
			}
			catch (std::exception ex)
			{
				return;
			}
			m_points.push_back(point);
		}
	}

	void Line::SetFill(bool _fill)
	{
		fill = _fill;
	}

	void Line::SetStrokeWidth(float width)
	{
		strokeWidth = width;
	}
}