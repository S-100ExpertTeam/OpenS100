#pragma once

#include "GM_Object.h"
#include "DirectPosition.h"

#include <vector>

namespace GM
{
	class MultiPoint : public Object
	{
	public:
		std::vector<DirectPosition> position;
	};
}