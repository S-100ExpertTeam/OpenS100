#pragma once
#include "TextPackageEnum.h"

namespace TextPackage
{
	class Font
	{
	public:
		Font();
		virtual ~Font();

	public:
		TextPackage::FontWeight weight = TextPackage::FontWeight::medium; // no initial value in the standard.
		TextPackage::FontSlant slant = TextPackage::FontSlant::upright; // no initial value in the standard.
		TextPackage::_FontSubClassType subClassType = TextPackage::_FontSubClassType::_Font;
	};
}