#pragma once
#include "Color.h"

namespace GraphicBasePackage
{
	class OverrideColor
	{
	public:
		OverrideColor();
		virtual ~OverrideColor();

	public:
		Color color;
	};
}