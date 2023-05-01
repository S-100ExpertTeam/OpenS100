#pragma once

#include "GM_Geometry.h"
#include "DirectPosition.h"

#include <vector>

namespace S100Geometry
{
	class MultiPoint : public Geometry
	{
	public:
		std::vector<DirectPosition> position;
	};
}