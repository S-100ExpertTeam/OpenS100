#pragma once
#include "Text.h"

namespace TextPackage
{
	class TextLine : public TextPackage::Text
	{
	public:
		TextLine();
		virtual ~TextLine();

	public:
		double startOffset = 0; // Standard has no initial value.
		double* endOffset = nullptr; // 0 .. * 
		SymbolPackage::LinePlacementMode placementMode = SymbolPackage::LinePlacementMode::relative; // Standard has no initial value.
	};
}
