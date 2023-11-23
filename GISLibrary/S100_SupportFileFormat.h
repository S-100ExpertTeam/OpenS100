#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

using namespace std;

namespace S100 {
	enum class S100_SupportFileFormat
	{
		TXT_UTF_8  = 1,
		JPEG2000,
		HTML,
		XML,
		XSLT,
		VIDEO,
		TIFF,
		PDF_AorUA,
		LUA,
		other = 100
	};

	inline string S100_SupportFileFormatToString(S100_SupportFileFormat category) {
		switch (category) {
		case S100_SupportFileFormat::TXT_UTF_8: return "TXT_UTF-8";
		case S100_SupportFileFormat::JPEG2000: return "JPEG2000";
		case S100_SupportFileFormat::HTML: return "HTML";
		case S100_SupportFileFormat::XML: return "XML";
		case S100_SupportFileFormat::XSLT: return "XSLT";
		case S100_SupportFileFormat::VIDEO: return "VIDEO";
		case S100_SupportFileFormat::TIFF: return "TIFF";
		case S100_SupportFileFormat::PDF_AorUA: return "PDF/AorUA";
		case S100_SupportFileFormat::LUA: return "LUA";
		case S100_SupportFileFormat::other: return "other";
		default: throw invalid_argument("Unknown category");
		}
	}

	inline S100_SupportFileFormat S100_SupportFileFormatFromString(const string& categoryName) {
		if (categoryName == "TXT_UTF-8") return S100_SupportFileFormat::TXT_UTF_8;
		if (categoryName == "JPEG2000") return S100_SupportFileFormat::JPEG2000;
		if (categoryName == "HTML") return S100_SupportFileFormat::HTML;
		if (categoryName == "XML") return S100_SupportFileFormat::XML;
		if (categoryName == "XSLT") return S100_SupportFileFormat::XSLT;
		if (categoryName == "VIDEO") return S100_SupportFileFormat::VIDEO;
		if (categoryName == "TIFF") return S100_SupportFileFormat::TIFF;
		if (categoryName == "PDF/AorUA") return S100_SupportFileFormat::PDF_AorUA;
		if (categoryName == "LUA") return S100_SupportFileFormat::LUA;
		
		return S100_SupportFileFormat::other;
	}


}