#pragma once

#include "..\\S100Engine\\Color.h"

#include "TextPackage_TextFlags.h"

#include <string>
#include <optional>

namespace TextPackage
{

	class TextElement
	{
	public:
		TextElement() = default;
		virtual ~TextElement() = default;

	private:
		std::string text;
		double bodySize = 10.0;
		double verticalOffset = 0.0;

		GraphicBasePackage::Color foreground;
		std::optional<GraphicBasePackage::Color> background;

	public:
		const std::string& GetText() const;
		void SetText(const std::string& text);

		double GetBodySize() const;
		void SetBodySize(double bodySize);

		double GetVerticalOffset() const;
		void SetVerticalOffset(double verticalOffset);

		const GraphicBasePackage::Color& GetForeground() const;
		void SetForeground(const GraphicBasePackage::Color& color);
		
		const std::optional<GraphicBasePackage::Color>& GetBackground() const;
		void SetBackground(const GraphicBasePackage::Color& color);
	};

}