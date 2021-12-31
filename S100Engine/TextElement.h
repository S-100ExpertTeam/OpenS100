#pragma once
#include "Font.h"
#include "TextFlags.h"
#include "GraphicBasePackage.h"

#include <string>

namespace TextPackage
{
	class TextElement
	{
	public:
		TextElement();
		virtual ~TextElement();

	public:
		std::wstring text;
		double bodySize = 10; // Standard has no initial value.
		double verticalOffset = 0;
		TextPackage::Font* font = nullptr; // FontReference or FontCharacteristics
		GraphicBasePackage::Color foreground;
		GraphicBasePackage::Color* background = nullptr; // 0 .. 1
		TextPackage::TextFlags* flags = nullptr; // 0 .. 1
	};
}
