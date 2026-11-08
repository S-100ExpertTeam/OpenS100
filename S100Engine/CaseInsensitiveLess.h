#pragma once
// Case-insensitive ordering for std::map keys. Replaces CStringCompareNoCaseLessThan (MFC).
#include <algorithm>
#include <cwctype>
#include <string>

namespace s100
{
	struct CaseInsensitiveLessW
	{
		using is_transparent = void;

		bool operator()(const std::wstring& lhs, const std::wstring& rhs) const
		{
			return std::lexicographical_compare(
				lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
				[](wchar_t a, wchar_t b) { return std::towlower(a) < std::towlower(b); });
		}
	};
}
