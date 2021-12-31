#pragma once
#include "Text.h"

namespace TextPackage
{
	class TextPoint : public::TextPackage::Text
	{
	public:
		TextPoint();
		virtual ~TextPoint();

	public:
		GraphicBasePackage::Vector* offset = nullptr; // 0 .. *
		double rotation = 0;
		SymbolPackage::AreaPlacementMode placementMode = SymbolPackage::AreaPlacementMode::visibleParts;
	};
}