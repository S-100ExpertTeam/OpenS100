#pragma once
#include <string>
#include <windows.h>

inline std::wstring toWide(const std::string& utf8)
{
	if (utf8.empty()) return L"";
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	if (size <= 1) return L"";
	std::wstring result(size - 1, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &result[0], size);
	return result;
}

inline std::string toUtf8(const std::wstring& wide)
{
	if (wide.empty()) return "";
	int size = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (size <= 1) return "";
	std::string result(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &result[0], size, nullptr, nullptr);
	return result;
}
