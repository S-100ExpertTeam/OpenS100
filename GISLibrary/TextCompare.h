#pragma once
// Case-insensitive comparison for std::wstring (replaces CString::CompareNoCase).
// Phase 1 step 3-2 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <cwctype>
#include <string_view>

namespace s100::text
{
	// <0, 0, >0 like CString::CompareNoCase; folds with towlower per code unit.
	inline int CompareNoCase(std::wstring_view a, std::wstring_view b)
	{
		const auto n = a.size() < b.size() ? a.size() : b.size();
		for (std::size_t i = 0; i < n; ++i)
		{
			const auto ca = std::towlower(static_cast<wint_t>(a[i]));
			const auto cb = std::towlower(static_cast<wint_t>(b[i]));
			if (ca != cb)
			{
				return ca < cb ? -1 : 1;
			}
		}
		if (a.size() == b.size())
		{
			return 0;
		}
		return a.size() < b.size() ? -1 : 1;
	}

	inline bool IEquals(std::wstring_view a, std::wstring_view b)
	{
		return CompareNoCase(a, b) == 0;
	}
}
