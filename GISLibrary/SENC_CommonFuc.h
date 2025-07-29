#pragma once

#include "SENC_Define.h"

#include "..\\S100Engine\\CRSType.h"

#include <string>

class SENC_CommonFuc
{
public:
	static int GetRotationCRS(std::wstring name);
	static int GetRotationCRS(GraphicBasePackage::CRSType value);
	static int GetCapStyle(std::wstring name);
	static int GetJoinStyle(std::wstring name);
	static int GetAreaCRSType(std::wstring name);
	static int GetHorizontalAlignment(std::wstring name);
	static int GetVerticalAlignment(std::wstring name);
	static int GetFontWeight(std::wstring name);
	static int GetFontSlant(std::wstring name);
	static int GetFontProportion(std::wstring name);
	static int GetLinePlacement(std::wstring name);
	static int GetAreaPlacement(std::wstring name);
};