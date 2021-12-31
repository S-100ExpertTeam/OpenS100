#pragma once
#include "figure.h"

namespace libS100Engine
{
	class Circle : public Figure
	{
	public:
		Circle();
		virtual ~Circle();
	
	public:
		double cx;
		double cy;
		double radius;

	private:
		void initialize();
	};
}