#include "stdafx.h"
#include "OrgPath.h"
#include "DrawingPathUnit.h"

namespace S100_SVG_D2D1_DLL
{
	OrgPath::OrgPath()
	{

	}

	OrgPath::~OrgPath()
	{
		std::vector<DrawingPathUnit*>::iterator itor;
		for (itor = dUnit.begin(); itor != dUnit.end(); itor++)
		{
			delete *itor;
		}
	}
}