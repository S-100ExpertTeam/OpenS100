#pragma once
#include "DrawingPathUnit.h"

namespace S100_SVG_D2D1_DLL 
{
	class DArc : public DrawingPathUnit
	{
	public:
		DArc();
		virtual ~DArc();

	public:
		float sx;
		float sy;
		double dwRadius;
		float eStartAngle;
		float eSweepAngle;
	};

}