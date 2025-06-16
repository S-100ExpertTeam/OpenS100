#pragma once
#include "Font.h"

namespace TextPackage
{
	class FontCharacteristics : public TextPackage::Font
	{
	public:
		FontCharacteristics();
		virtual ~FontCharacteristics();

	public:
		bool serifs = false; // no initial value in the standard.
		TextPackage::FontProportion proportion = TextPackage::FontProportion::monoSpaced; // no initial value in the standard.
	};
}