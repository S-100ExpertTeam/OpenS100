#pragma once

#include "GM_OrientableCurve.h"
#include "GM_CurveBoundary.h"
#include "GM_CurveSegment.h"

#include <vector>

namespace GM
{
	class Curve : public OrientableCurve
	{
	public:
		Curve();
		Curve(const Curve& item);
		virtual ~Curve();

	public:
		CurveBoundary boundary;
		std::vector<CurveSegment> segment;

	public:
		GeometryType GetType() override;

		void Add(double x, double y);

		bool IsClosed();
		
		int getPointCount() override;
		double getX(int index) override;
		double getY(int index) override;

		SGeometry* createGeometry();
	};
}