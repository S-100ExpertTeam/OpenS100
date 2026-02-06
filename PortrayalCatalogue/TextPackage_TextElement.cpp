#include "stdafx.h"
#include "TextPackage_TextElement.h"

namespace TextPackage
{
	const std::string& TextElement::GetText() const
	{
		return text;
	}

	void TextElement::SetText(const std::string& text)
	{
		this->text = text;
	}

	double TextElement::GetBodySize() const
	{
		return bodySize;
	}

	void TextElement::SetBodySize(double bodySize)
	{
		this->bodySize = bodySize;
	}

	double TextElement::GetVerticalOffset() const
	{
		return verticalOffset;
	}

	void TextElement::SetVerticalOffset(double verticalOffset)
	{
		this->verticalOffset = verticalOffset;
	}

	void TextElement::SetForeground(const GraphicBasePackage::Color& color)
	{ 
		foreground = color; 
	}

	void TextElement::SetBackground(const GraphicBasePackage::Color& color) 
	{ 
		background = color; 
	}

	const GraphicBasePackage::Color& TextElement::GetForeground() const 
	{ 
		return foreground; 
	}

	const std::optional<GraphicBasePackage::Color>& TextElement::GetBackground() const 
	{ 
		return background; 
	}
}
