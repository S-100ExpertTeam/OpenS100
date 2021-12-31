#pragma once

#include "ColorMap.h"
#include "Color.h"

namespace nmColorMap {

	class ColorMapCalculator
	{
	public:
		ColorMapCalculator();
		virtual ~ColorMapCalculator();
	
	public:
		nmColorMap::ColorMap Calculate(nmColorMap::ColorMap _colorMap, int _num);
		nmColorMap::Color Calculate(nmColorMap::Color _color1, nmColorMap::Color _color2, double _percent);
	};

}