#pragma once
#include <string_view>

namespace GISLibrary
{
	// Diagnostic text for a developer: the debugger output window on Windows, stderr elsewhere.
	void DebugOutput(std::string_view text);
	void DebugOutput(std::wstring_view text);
}
