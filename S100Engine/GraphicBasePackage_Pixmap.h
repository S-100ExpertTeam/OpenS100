#pragma once
#include "Color.h"
#include "GraphicBasePackage_OverrideColor.h"

#include <vector>

namespace GraphicBasePackage
{
	class Pixmap
	{
	public:
		Pixmap();
		virtual ~Pixmap();

	public:
		std::wstring reference;
		Color* overrideAll = nullptr;
		std::vector<OverrideColor* > override;
	};
}