#pragma once

#include "TextPackageEnum.h"

#include <vector>

namespace TextPackage
{
	class TextFlags
	{
	public:
		TextFlags();
		virtual ~TextFlags();
	public:
		std::vector<TextPackage::TextFlag> flag; // 1 .. *
	};
}