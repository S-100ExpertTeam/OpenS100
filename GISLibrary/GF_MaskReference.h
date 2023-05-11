#pragma once

#include "GM_Object.h"

namespace GF
{
	class MaskReference
	{
	public:
		MaskReference();
		virtual ~MaskReference();

	public:

		GM::Object* geometry = nullptr;
	};
}