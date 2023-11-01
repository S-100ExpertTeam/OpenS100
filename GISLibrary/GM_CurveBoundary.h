#pragma once

#include "GM_Point.h"

namespace GM
{
	class CurveBoundary
	{
	public:
		CurveBoundary();
		CurveBoundary(CurveBoundary& item);
		virtual ~CurveBoundary();

	public:
		Point* startPoint = nullptr;
		Point* endPoint = nullptr;
	};
}