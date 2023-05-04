#pragma once

#include "GM_Object.h"
#include "DirectPosition.h"

#include <vector>

namespace S100Geometry
{
	class MultiPoint : public Object
	{
	public:
		std::vector<DirectPosition> position;
	};
}