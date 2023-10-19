#include "stdafx.h"
#include "GM_CurveBoundary.h"

namespace GM
{
	CurveBoundary::CurveBoundary()
	{

	}

	CurveBoundary::CurveBoundary(CurveBoundary& item)
	{
		this->startPoint = item.startPoint;
		this->endPoint = item.endPoint;
	}

	CurveBoundary::~CurveBoundary()
	{
		delete startPoint;
		startPoint = nullptr;

		delete endPoint;
		endPoint = nullptr;
	}
}