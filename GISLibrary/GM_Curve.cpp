#include "stdafx.h"
#include "GM_Curve.h"

namespace GM
{
	Curve::Curve()
	{
		segment.push_back(CurveSegment());
	}

	Curve::Curve(const Curve& item) : OrientableCurve(item)
	{
		segment.push_back(CurveSegment());
		this->boundary = item.boundary;
		this->segment = item.segment;
	}

	Curve::~Curve()
	{

	}

	GeometryType Curve::GetType()
	{
		return GeometryType::Curve;
	}

	void Curve::Add(double x, double y)
	{
		segment.front().controlPoints.push_back(DirectPosition(x, y));
	}

	bool Curve::IsClosed()
	{
		if (segment.front().controlPoints.front() == segment.front().controlPoints.back())
		{
			return true;
		}

		return false;
	}

	int Curve::getPointCount()
	{
		return (int)segment.front().controlPoints.size();
	}

	double Curve::getX(int index)
	{
		auto pointCount = getPointCount();

		if (index < 0 || index >= pointCount) {
			return 0;
		}

		return segment.front().controlPoints.at(index).GetX();
	}

	double Curve::getY(int index)
	{
		auto pointCount = getPointCount();

		if (index < 0 || index >= pointCount) {
			return 0;
		}

		return segment.front().controlPoints.at(index).GetY();
	}

	SGeometry* Curve::createGeometry()
	{
		SCurve* result = new SCurve();
		auto pointCount = getPointCount();
		result->Init(pointCount);

		for (auto i = 0; i < pointCount; i++) {
			auto x = getX(i);
			auto y = getY(i);
			projection(x, y);

			result->Set(i, x, y);
		}

		return result;
	}
}