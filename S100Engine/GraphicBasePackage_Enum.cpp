#include "stdafx.h"

#include "GraphicBasePackage_Enum.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace GraphicBasePackage
{
	CRSType GetCRSTypeFromString(const std::string& value)
	{
		auto lower = LatLonUtility::To_Lowercase(value);
		if (lower == "geographiccrs")
		{
			return CRSType::geographicCRS;
		}
		else if (lower == "portrayalcrs")
		{
			return CRSType::portrayalCRS;
		}
		else if (lower == "localcrs")
		{
			return CRSType::localCRS;
		}
		else if (lower == "linecrs")
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