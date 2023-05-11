#pragma once

#include "GM_Primitive.h"

#include "DirectPosition.h"

#include <string>

namespace GM
{
	class Point : public Primitive
	{
	public:
		DirectPosition position;
	};
}