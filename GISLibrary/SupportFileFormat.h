#pragma once

#include <string>

namespace S100
{

	enum class SupportFileFormat
	{
		none = 0,
		ASCII = 1,
		JPEG2000 = 2,
		HTML = 3,
		XML = 4,
		XSLT = 5,
		VIDEO = 6,
		TIFF = 7,
		PDF_AorUA = 8,
		LUA = 9,
		other = 100
	};

	SupportFileFormat stringToSupportFileFormat(std::string value);
	std::string supportFileFormatToString(SupportFileFormat value);
}