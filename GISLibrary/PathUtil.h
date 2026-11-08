#pragma once
// Path helpers for the core (replace LibMFCUtil::GetExtension/GetFileName and ad-hoc CString slicing).
//
// Both separators are recognised. Results are plain std::wstring so callers stay simple;
// use std::filesystem::path directly when you need more than a component.
// Phase 1 step 3-2 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <filesystem>
#include <string>
#include <string_view>

namespace s100::path
{
	// Extension without the dot ("000" for "a\\b.000"); empty when there is none.
	std::wstring Extension(std::wstring_view path);

	// Last component including its extension ("b.000").
	std::wstring FileName(std::wstring_view path);

	// Last component without its extension ("b"); the whole component when it has no dot.
	std::wstring Stem(std::wstring_view path);

	// Directory part including the trailing separator; empty when there is none.
	std::wstring Directory(std::wstring_view path);

	// True for rooted paths (drive or UNC on Windows, leading '/' elsewhere).
	bool IsAbsolute(std::wstring_view path);

	// Directory that holds the running executable (current path when unavailable).
	std::filesystem::path ExecutableDirectory();
}
