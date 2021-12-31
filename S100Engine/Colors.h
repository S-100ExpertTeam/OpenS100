#pragma once
#include "Color.h"

#include <vector>

namespace libS100Engine
{
	class Colors
	{
	public:
		Colors();
		virtual ~Colors();
	
	public:
		std::vector<Color> colors;
	};
}