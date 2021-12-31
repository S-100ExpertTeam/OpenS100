#pragma once
#include "figure.h"

#include <vector>

namespace libS100Engine
{

	class Line : public Figure
	{
	public:
		Line();
		Line(std::vector<std::wstring> points);
		Line(POINTF points[], int pointsCount);
		virtual ~Line();

	public:
		std::vector<POINTF> m_points;

	public:
		void AddPoint(POINTF point);
		void AddPoints(std::vector<std::wstring> points);
		void SetFill(bool fill);
		void SetStrokeWidth(float width);

	private:
		void initialize();
	};
}