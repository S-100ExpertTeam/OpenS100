#pragma once
#include "OrgDrawingPattern.h"

namespace S100_SVG_D2D1_DLL
{
	class Path;
	class OrgArc : public OrgDrawingPattern
	{
	public:
		OrgArc();
		virtual ~OrgArc();
	public:
		Path* orgValue;
		double rx, ry;
		int x_axis_rotation;
		int large_arc_flag, sweep_flag;
		double dx, dy;
	};
}