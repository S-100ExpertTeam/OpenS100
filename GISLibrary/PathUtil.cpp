#include "stdafx.h"
#include "PathUtil.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace
{
	constexpr std::wstring_view kSeparators = L"\\/";

	std::size_t LastSeparator(std::wstring_view path)
	{
		return path.find_last_of(kSeparators);
	}

	std::wstring_view LastComponent(std::wstring_view path)
	{
		const auto sep = LastSeparator(path);
		return sep == std::wstring_view::npos ? path : path.substr(sep + 1);
	}
}

namespace s100::path
{
	std::wstring Extension(std::wstring_view path)
	{
		const auto name = LastComponent(path);
		const auto dot = name.rfind(L'.');
		return dot == std::wstring_view::npos ? std::wstring() : std::wstring(name.substr(dot + 1));
	}

	std::wstring FileName(std::wstring_view path)
	{
		return std::wstring(LastComponent(path));
	}

	std::wstring Stem(std::wstring_view path)
	{
		const auto name = LastComponent(path);
		const auto dot = name.rfind(L'.');
		return std::wstring(dot == std::wstring_view::npos ? name : name.substr(0, dot));
	}

	std::wstring Directory(std::wstring_view path)
	{
		const auto sep = LastSeparator(path);
		return sep == std::wstring_view::npos ? std::wstring() : std::wstring(path.substr(0, sep + 1));
	}

	bool IsAbsolute(std::wstring_view path)
	{
		return std::filesystem::path(path).is_absolute();
	}

	std::filesystem::path ExecutableDirectory()
	{
#ifdef _WIN32
		wchar_t buffer[MAX_PATH]{};
		const DWORD length = ::GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		if (length > 0 && length < MAX_PATH)
		{
			return std::filesystem::path(buffer).parent_path();
		}
#endif
		std::error_code ec;
		auto current = std::filesystem::current_path(ec);
		return ec ? std::filesystem::path() : current;
	}
}
