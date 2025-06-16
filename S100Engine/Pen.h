#pragma once
#include "Color.h"

#include <string>

namespace GraphicBasePackage
{	
	class Pen
	{
	public:
		Pen();
		virtual ~Pen();
	public:
		double width = 0.32;
		GraphicBasePackage::Color color;

	public:
		void setWidth(double value);
		double getWidth();

		void setColor(GraphicBasePackage::Color value);
		GraphicBasePackage::Color getColor();
	};
}