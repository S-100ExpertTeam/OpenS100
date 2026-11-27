#pragma once
#include "GraphicBasePackage_Enum.h"
#include "Pen.h"
#include "Color.h"

namespace GraphicBasePackage
{
	class Vector
	{
	public:
		Vector();
		Vector(double x, double y);
		virtual ~Vector();

	public:
		double x = 0; // no initial value in the standard.
		double y = 0;

	public:
		double GetX() const;
		void SetX(double value);

		double GetY() const;
		void SetY(double value);

		void Set(double x, double y);
	};
}