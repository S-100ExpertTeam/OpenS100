#pragma once
#include "DrawingPathUnit.h"

namespace S100_SVG_D2D1_DLL
{
	class DPoint : public DrawingPathUnit
	{
	public:
		DPoint();
		virtual ~DPoint();

	public:
		float x;
		float y;
	};
}