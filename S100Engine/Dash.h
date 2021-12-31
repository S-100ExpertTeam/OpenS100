#pragma once

namespace LineStylesPackage
{
	class Dash
	{
	public:
		Dash();
		virtual ~Dash();
	public:
		double start = 0;
		double length = 0;
	};
}