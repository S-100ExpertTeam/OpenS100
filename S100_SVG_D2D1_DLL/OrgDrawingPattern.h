#pragma once
#include <list>

namespace S100_SVG_D2D1_DLL 
{
	class CSSStyle;
	class OrgDrawingPattern
	{
	public:
		OrgDrawingPattern();
		virtual ~OrgDrawingPattern();

	public:
		//////////////////////////////////////////
		// 1: point 
		// 2: polyline
		// 3: polygon
		// 4: circle
		int type;
		///////////////////////////////////////////
		// is Fill the figure or not
		bool fill;
		COLORREF *color;
		//////////////////////////////////////////
		//*  size of width
		int width;
		//////////////////////////////////////////
	};
}