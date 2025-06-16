#pragma once
#include "stdafx.h"
#include "Pen.h"

namespace GraphicBasePackage
{
	Pen::Pen()
	{
		
	}

	Pen::~Pen()
	{

	}

	void Pen::setWidth(double value)
	{
		width = value;
	}

	double Pen::getWidth()
	{
		return width;
	}

	void Pen::setColor(GraphicBasePackage::Color value)
	{
		color = value;
	}

	GraphicBasePackage::Color Pen::getColor()
	{
		return color;
	}
}