#pragma once

#include "resource.h"

#include <string>
#include <tchar.h>
#include <atlconv.h>
#include <vector>

namespace GISLibrary {
	static inline int string2int(std::string value)
	{
		try
		{
			return std::stoi(value);
		}
		catch (std::exception e)
		{
			return 0;
		}

		return 0;
	}

	static inline float string2float(std::string value)
	{
		try
		{
			return std::stof(value);
		}
		catch (std::exception e)
		{
			return 0.0;
		}

		return 0.0;
	}

	static inline double string2double(std::string value)
	{
		try
		{
			return std::stod(value);
		}
		catch (std::exception e)
		{
			return 0.0;
		}

		return 0.0;
	}

	static inline std::wstring string2wstring(const std::string& str)
	{
		if (str.empty())
			return _T("");

		USES_CONVERSION_EX;
		LPCWSTR wideStr = A2CW_EX(str.c_str(), str.length());
		if (wideStr == nullptr)
			return _T("");

		return std::wstring(wideStr);
	}

	static inline std::string wstring2string(const std::wstring& str)
	{
		if (str.empty())
			return "";

		USES_CONVERSION_EX;
		LPSTR wideStr = W2A_EX(str.c_str(), str.length());
		if (wideStr == nullptr)
			return "";

		return std::string(wideStr);
	}

	static inline std::vector<std::string> split(const std::string& str, char delimiter)
	{
		std::vector<std::string> tokens;
		size_t start = 0, end;

		while ((end = str.find(delimiter, start)) != std::string::npos) {
			if (end > start) { // Ignore empty substrings
				tokens.push_back(str.substr(start, end - start));
			}
			start = end + 1;
		}

		// Add the last token if it's not empty
		if (start < str.size()) {
			tokens.push_back(str.substr(start));
		}

		return tokens;
	}
}

