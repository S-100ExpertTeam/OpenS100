#include "stdafx.h"
#include "DebugOutput.h"

#include "../GeoMetryLibrary/TextCodec.h"

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <string>

namespace GISLibrary
{
	void DebugOutput(std::string_view text)
	{
#ifdef _WIN32
		::OutputDebugStringA(std::string(text).c_str());
#else
		std::clog << text;
#endif
	}

	void DebugOutput(std::wstring_view text)
	{
#ifdef _WIN32
		::OutputDebugStringW(std::wstring(text).c_str());
#else
		std::clog << s100::text::ToUtf8(text);
#endif
	}
}
