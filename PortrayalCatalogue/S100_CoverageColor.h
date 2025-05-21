#pragma once

#include "..\\S100Engine\\Color.h"

#include <optional>

class S100_CoverageColor
{
public:
	S100_CoverageColor();
	virtual ~S100_CoverageColor();

public:
	std::optional<double> penWidth;
	GraphicBasePackage::Color startColor;
	std::optional<GraphicBasePackage::Color> endColor;

public:
	void setPenWidth(double value);
	std::optional<double> getPenWidth();

	void setStartColor(GraphicBasePackage::Color value);
	GraphicBasePackage::Color getStartColor();

	void setEndColor(GraphicBasePackage::Color value);
	std::optional<GraphicBasePackage::Color> getEndColor();
};

