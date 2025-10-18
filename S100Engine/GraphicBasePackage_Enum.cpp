#include "stdafx.h"
#include "GraphicBasePackage_Enum.h"

namespace GraphicBasePackage
{
	CRSType GetCRSTypeFromString(const std::string& value)
	{
		if (value == "geographicCRS")
		{
			return CRSType::geographicCRS;
		}
		else if (value == "portrayalCRS")
		{
			return CRSType::portrayalCRS;
		}
		else if (value == "localCRS")
		{
			return CRSType::localCRS;
		}
		else if (value == "lineCRS")
		{
			return CRSType::lineCRS;
		}

		return CRSType::CRSType_None;
	}

	CRSType GetCRSTypeFromString(const std::wstring& value)
	{
		return GetCRSTypeFromString(std::string(value.begin(), value.end()));
	}
}