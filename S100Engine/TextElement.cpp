#include "stdafx.h"
#include "TextElement.h"

namespace TextPackage
{
	TextElement::TextElement()
	{

	}

	TextElement::~TextElement()
	{
		if (font)
		{
			delete font;
			font = nullptr;
		}

		if (background)
		{
			delete background;
			background = nullptr;
		}

		if (flags)
		{
			delete flags;
			flags = nullptr;
		}
	}
}