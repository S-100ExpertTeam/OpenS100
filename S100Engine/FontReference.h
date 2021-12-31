#pragma once
#include "Font.h"

#include <string>

namespace TextPackage
{
	class FontReference : public TextPackage::Font
	{
	public:
		FontReference();
		virtual ~FontReference();
		
	public:
		std::wstring reference; // no initial value in the standard.
	};
}

