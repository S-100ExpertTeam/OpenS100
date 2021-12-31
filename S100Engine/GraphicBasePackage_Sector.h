#pragma once
#include "GraphicBasePackage_Enum.h"

namespace GraphicBasePackage
{
	class Sector
	{
	public:
		Sector();
		virtual ~Sector();

	public:
		CRSType _rotationCRS = CRSType::geographicCRS;
		double _startAngle = 0;
		double _angularDistance = 0;
	};
}
