#pragma once

namespace GraphicBasePackage
{
	enum class CRSType
	{
		CRSType_None = 0,
		geographicCRS = 1,
		portrayalCRS,
		localCRS,
		lineCRS
	};

	CRSType GetCRSTypeFromString(const std::string& value);
	CRSType GetCRSTypeFromString(const std::wstring& value);
}