#include "stdafx.h"
#include "SupportFileFormat.h"

namespace S100
{

	std::string SupportFileFormatToString(SupportFileFormat value)
	{
		switch (value)
		{
		case SupportFileFormat::none: return "";
		case SupportFileFormat::ASCII: return "ASCII";
		case SupportFileFormat::JPEG2000: return "JPEG2000";
		case SupportFileFormat::HTML: return "HTML";
		case SupportFileFormat::XML: return "XML";
		case SupportFileFormat::XSLT: return "XSLT";
		case SupportFileFormat::VIDEO: return "VIDEO";
		case SupportFileFormat::TIFF: return "TIFF";
		case SupportFileFormat::PDF_AorUA: return "PDF/AorUA";
		case SupportFileFormat::LUA: return "LUA";
		case SupportFileFormat::other: return "other";
		default: return "";
		}
	}

	SupportFileFormat SupportFileFormatFromString(const std::string value)
	{
		if (value == "ASCII") return SupportFileFormat::ASCII;
		else if (value == "JPEG2000") return SupportFileFormat::JPEG2000;
		else if (value == "HTML") return SupportFileFormat::HTML;
		else if (value == "XML") return SupportFileFormat::XML;
		else if (value == "XSLT") return SupportFileFormat::XSLT;
		else if (value == "VIDEO") return SupportFileFormat::VIDEO;
		else if (value == "TIFF") return SupportFileFormat::TIFF;
		else if (value == "PDF/AorUA") return SupportFileFormat::PDF_AorUA;
		else if (value == "LUA") return SupportFileFormat::LUA;
		else if (value == "none") return SupportFileFormat::other;
		else return SupportFileFormat::none;
	}

}
