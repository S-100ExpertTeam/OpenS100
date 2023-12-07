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
	string S100_SupportFileFormatToString(S100_SupportFileFormat category);
	S100_SupportFileFormat S100_SupportFileFormatFromString(const string& categoryName);
}