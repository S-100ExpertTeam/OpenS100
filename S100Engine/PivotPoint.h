#pragma once
#include "figure.h"

namespace libS100Engine
{
	class PivotPoint : public Figure
	{
	public:
		PivotPoint();
		virtual ~PivotPoint();

	public:
		double cx;
		double cy;
		double radius;

	private:
		void initialize();
	};
}