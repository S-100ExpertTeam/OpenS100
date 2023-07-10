#pragma once

#include <string>

namespace libS100Engine
{
	enum class FigureType
	{
		none = 0,
		line = 1,
		circle = 2,
		rect = 3,
		pivotPoint = 4
	};

	class Figure
	{
	public:
		Figure();
		virtual ~Figure();

	public:
		FigureType type;
		std::wstring colorName;
		double strokeWidth;
		std::wstring strokeDasharray;
		int rotation;
		bool fill;
		float alpha = 1.0f;
	};
}