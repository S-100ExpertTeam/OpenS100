#pragma once

#include "GM_OrientableCurve.h"

#include <vector>

namespace GM
{
	class CompositeCurve : public OrientableCurve
	{
	public:
		std::vector<OrientableCurve> component;

	public:
		GeometryType GetType() override;
	};
}