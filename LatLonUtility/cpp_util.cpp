#include "cpp_util.h"

namespace cpp_util
{
	double stod(const std::string& s, double defaultVal)
	{
		try {
			return s.empty() ? defaultVal : std::stod(s);
		}
		catch (...) {
			return defaultVal;
		}
	}

	double stod(const std::wstring& s, double defaultVal)
	{
		try {
			return s.empty() ? defaultVal : std::stod(s);
		}
		catch (...) {
			return defaultVal;
		}
	}

	int stoi(const std::string& s, int defaultVal)
	{
		try {
			return s.empty() ? defaultVal : std::stoi(s);
		}
		catch (...) {
			return defaultVal;
		}
	}

	int stoi(const std::wstring& s, int defaultVal)
	{
		try {
			return s.empty() ? defaultVal : std::stoi(s);
		}
		catch (...) {
			return defaultVal;
		}
	}
}