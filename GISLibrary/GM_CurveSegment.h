#pragma once

#include "DirectPosition.h"

#include <vector>

namespace S100Geometry
{
	class CurveSegment
	{
	public:
		std::vector<DirectPosition> controlPoints;
	};
}