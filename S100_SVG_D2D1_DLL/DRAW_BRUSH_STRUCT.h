#pragma once

namespace S100_SVG_D2D1_DLL 
{
	class DRAW_BRUSH_STRUCT
	{
	public:
		DRAW_BRUSH_STRUCT()
		{

		}

		DRAW_BRUSH_STRUCT(bool _FILL_POLYGON, int _BRUSH_COLOR)
		{
			FILL_POLYGON = _FILL_POLYGON;
			BRUSH_COLOR = _BRUSH_COLOR;
		}

		virtual ~DRAW_BRUSH_STRUCT()
		{

		}

	public:
		bool FILL_POLYGON = false;
		int BRUSH_COLOR = 0;
	};
}