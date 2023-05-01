#pragma once

#include "GM_OrientableCurve.h"

#include <vector>

namespace S100Geometry
{
	class CompositeCurve : public OrientableCurve
	{
	public:
		std::vector<OrientableCurve> component;

	public:
		GeometryType GetType() override;
	};
}