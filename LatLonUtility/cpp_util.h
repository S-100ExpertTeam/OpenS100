#pragma once

#include <string>

namespace cpp_util
{
	/**
	 * @brief Safely convert a string to a double, returning a default value if conversion fails
	 * @param s Input string to convert
	 * @param defaultVal Value to return if conversion fails (default: 0.0)
	 * @return Converted double value or defaultVal if conversion fails
	 */
	double stod(const std::string& s, double defaultVal = 0.0);
	double stod(const std::wstring& s, double defaultVal = 0.0);

	int stoi(const std::string& s, int defaultVal = 0);
	int stoi(const std::wstring& s, int defaultVal = 0);
}