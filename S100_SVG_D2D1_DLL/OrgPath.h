#pragma once

#include "OrgDrawingPattern.h"
#include <vector>

namespace S100_SVG_D2D1_DLL 
{	
	class Path;
	class DrawingPathUnit;
	class OrgPath : public OrgDrawingPattern
	{
	public:
		OrgPath();
		virtual ~OrgPath();

	public:
		//////////////////////////////////////////
		Path* orgValue;
		//////////////////////////////////////////
		// coordinates information
		std::vector<DrawingPathUnit*> dUnit;
	};
}