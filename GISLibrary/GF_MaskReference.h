#pragma once

#include "GM_Object.h"
#include "GF_MaskIndicatorType.h"

namespace GF
{
	class MaskReference
	{
	public:
		MaskReference();
		virtual ~MaskReference();

	public:

		//GM::Object* geometry = nullptr;
		std::string geometryID;
		MaskIndicatorType maskIndicator;
	};
}