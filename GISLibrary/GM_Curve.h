#pragma once

#include "GM_OrientableCurve.h"
#include "GM_CurveBoundary.h"
#include "GM_CurveSegment.h"

#include <vector>

namespace S100Geometry
{
	class Curve : public OrientableCurve
	{
	public:
		Curve();
		~Curve();

	public:
		CurveBoundary boundary;
		std::vector<CurveSegment> segment;

	public:
		GeometryType GetType() override;
	};
}