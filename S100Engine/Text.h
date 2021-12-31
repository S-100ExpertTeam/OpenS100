#pragma once

#include "TextPackageEnum.h"
#include "GraphicBasePackage.h"
#include "SymbolPackageEnum.h"

namespace TextPackage
{
	class TextElement;
	class TextFlags;
	class Font;
	class Text
	{
	public:
		Text();
		virtual ~Text();

	public:
		TextPackage::HorizontalAlignment horizontalAlignment = TextPackage::HorizontalAlignment::start;
		TextPackage::VerticalAlignment verticalAlignment = TextPackage::VerticalAlignment::bottom;
		std::vector<TextElement*> element; // 1 .. *
	};
}