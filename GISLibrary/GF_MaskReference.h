#pragma once

#include "GM_Geometry.h"

namespace GF
{
	class MaskReference
	{
	public:
		MaskReference();
		virtual ~MaskReference();

	public:

		S100Geometry::Geometry* geometry = nullptr;
	};
}