#include "stdafx.h"
#include "S100_CoverageColor.h"

S100_CoverageColor::S100_CoverageColor()
{

}

S100_CoverageColor::~S100_CoverageColor()
{
}

void S100_CoverageColor::setPenWidth(double value)
{
	penWidth = value;
}

std::optional<double> S100_CoverageColor::getPenWidth()
{
	return penWidth;
}

void S100_CoverageColor::setStartColor(GraphicBasePackage::Color value)
{
	startColor = value;
}

GraphicBasePackage::Color S100_CoverageColor::getStartColor()
{
	return startColor;
}

void S100_CoverageColor::setEndColor(GraphicBasePackage::Color value)
{
	endColor = value;
}

std::optional<GraphicBasePackage::Color> S100_CoverageColor::getEndColor()
{
	return endColor;
}
