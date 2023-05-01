#pragma once

#include "GM_Primitive.h"

#include "DirectPosition.h"

#include <string>

namespace S100Geometry
{
	class Point : public Primitive
	{
	public:
		DirectPosition position;
	};
}