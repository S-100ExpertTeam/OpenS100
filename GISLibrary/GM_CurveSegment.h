#pragma once

#include "DirectPosition.h"

#include <vector>

namespace GM
{
	class CurveSegment
	{
	public:
		std::vector<DirectPosition> controlPoints;
	};
}