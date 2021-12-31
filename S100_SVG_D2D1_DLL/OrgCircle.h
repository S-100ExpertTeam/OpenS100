#pragma once
#include "OrgDrawingPattern.h"

namespace S100_SVG_D2D1_DLL
{
	class Circle;
	class OrgCircle : public OrgDrawingPattern
	{
	public:
		OrgCircle();
		virtual ~OrgCircle();

	public:
		//////////////////////////////////////////
		Circle* orgValue;
		//////////////////////////////////////////
		// coordinates information
		float center_x;
		float center_y;
		float radius;
	};
}