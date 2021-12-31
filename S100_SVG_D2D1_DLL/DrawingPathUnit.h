#pragma once
namespace S100_SVG_D2D1_DLL 
{
	class DrawingPathUnit
	{
	public:
		DrawingPathUnit();
		virtual ~DrawingPathUnit();
		// type		1: point
		//			2: arc
	public:
		int type = 0;
	};
}