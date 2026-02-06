#pragma once

namespace Part9a
{

	class Vector
	{
	public:
		Vector() = default;

	public:
		double x = 0.0;
		double y = 0.0;

	public:
		void SetX(double value);
		double GetX() const;

		void SetY(double value);
		double GetY() const;

		void Set(double x, double y);
	};

}