#pragma once
#include "LineStylesPackageEnum.h"
#include "GraphicBasePackage.h"

#include <string>

namespace LineStylesPackage
{
	class LineSymbol
	{
	public:
		LineSymbol();
		virtual ~LineSymbol();
	
	public:
		std::wstring reference;
		double _rotation = 0;
		double _scaleFactor = 1;
		GraphicBasePackage::CRSType _crsType = GraphicBasePackage::CRSType::localCRS;
		CapStyle capStyle;
		JoinStyle joinStyle;
		double position;
	};	
}