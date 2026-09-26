#pragma once
#include <string>
#include <utf8cpp/utf8.h>

inline std::wstring toWide(const std::string& utf8)
{
	if (utf8.empty()) return L"";
	std::u16string u16;
	utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(u16));
	return std::wstring(u16.begin(), u16.end());
}

inline std::string toUtf8(const std::wstring& wide)
{
	if (wide.empty()) return "";
	std::u16string u16(wide.begin(), wide.end());
	std::string result;
	utf8::utf16to8(u16.begin(), u16.end(), std::back_inserter(result));
	return result;
}
