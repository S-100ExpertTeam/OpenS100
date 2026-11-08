#pragma once
// Platform-neutral UTF-8 <-> wide-string conversion for the core projects.
//
// Replaces LibMFCUtil::ConvertWCtoC / ConvertCtoWC (Win32 WideCharToMultiByte) so that
// GeoMetryLibrary has no MFC/Win32 dependency. wchar_t is UTF-16 on Windows and UTF-32
// elsewhere; both are handled. Malformed input is replaced with U+FFFD.
// Phase 1 step 3-4 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

namespace s100::text
{
	inline std::wstring ToWide(std::string_view utf8)
	{
		std::wstring out;
		out.reserve(utf8.size());

		size_t i = 0;
		while (i < utf8.size())
		{
			const auto lead = static_cast<unsigned char>(utf8[i]);
			char32_t cp = 0xFFFD;
			size_t len = 1;
			if (lead < 0x80) { cp = lead; }
			else if ((lead & 0xE0) == 0xC0) { cp = lead & 0x1F; len = 2; }
			else if ((lead & 0xF0) == 0xE0) { cp = lead & 0x0F; len = 3; }
			else if ((lead & 0xF8) == 0xF0) { cp = lead & 0x07; len = 4; }

			if (i + len > utf8.size())
			{
				cp = 0xFFFD;
				len = 1;
			}
			else
			{
				for (size_t k = 1; k < len; ++k)
				{
					const auto trail = static_cast<unsigned char>(utf8[i + k]);
					if ((trail & 0xC0) != 0x80)
					{
						cp = 0xFFFD;
						len = k;
						break;
					}
					cp = (cp << 6) | (trail & 0x3F);
				}
			}
			i += len;

			if constexpr (sizeof(wchar_t) == 2)
			{
				if (cp >= 0x10000)
				{
					cp -= 0x10000;
					out.push_back(static_cast<wchar_t>(0xD800 + (cp >> 10)));
					out.push_back(static_cast<wchar_t>(0xDC00 + (cp & 0x3FF)));
					continue;
				}
			}
			out.push_back(static_cast<wchar_t>(cp));
		}
		return out;
	}

	inline std::string ToUtf8(std::wstring_view wide)
	{
		using UnsignedWide = std::make_unsigned_t<wchar_t>;
		std::string out;
		out.reserve(wide.size() * 3);

		for (size_t i = 0; i < wide.size(); ++i)
		{
			char32_t cp = static_cast<UnsignedWide>(wide[i]);
			if constexpr (sizeof(wchar_t) == 2)
			{
				if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < wide.size())
				{
					const char32_t low = static_cast<UnsignedWide>(wide[i + 1]);
					if (low >= 0xDC00 && low <= 0xDFFF)
					{
						cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
						++i;
					}
				}
			}

			if (cp < 0x80)
			{
				out.push_back(static_cast<char>(cp));
			}
			else if (cp < 0x800)
			{
				out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
				out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
			else if (cp < 0x10000)
			{
				out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
				out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
			else
			{
				out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
				out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
		}
		return out;
	}
}
