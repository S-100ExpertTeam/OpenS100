#pragma once

#include "GM_Object.h"
#include "GF_MaskIndicatorType.h"

namespace GF
{
	class MaskReference
	{
	public:
		MaskReference();
		MaskReference(const MaskReference& other);
		virtual ~MaskReference();

	public:

		//GM::Object* geometry = nullptr;
		std::string geometryID;
		MaskIndicatorType maskIndicator;

	public:
		virtual MaskReference operator=(const MaskReference& other);
	};
}