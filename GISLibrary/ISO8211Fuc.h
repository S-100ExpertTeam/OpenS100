#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include "BinaryWriter.h"
#include "NonPrintableCharacter.h"
#include "../GeoMetryLibrary/TextCodec.h"

// Reads a UTF-8 text subfield up to the unit terminator (consumed) into a wide string.
void buf2charArr(std::wstring& dest, BYTE*& buf);
// Reads exactly `len` bytes of UTF-8 text into a wide string.
void buf2charArr(std::wstring& dest, BYTE*& buf, int len);
DWORD buf2uint(BYTE*& buf, int len);
double buf2double(BYTE*& buf, int size);
DWORD buf2int(BYTE*& buf, int len);
DWORD atoi(BYTE*& buf, int len);

int asNumeric( wchar_t wch );
wchar_t const* const digitTables[] =
{
    L"0123456789",
    L"\u0660\u0661\u0662\u0663\u0664\u0665\u0666\u0667\u0668\u0669",
};
namespace s100::iso8211
{
	// Byte length of `text` once encoded as UTF-8 (what WriteUtf8 emits).
	inline std::size_t Utf8Length(std::wstring_view text)
	{
		return s100::text::ToUtf8(text).size();
	}

	// Writes `text` as UTF-8 without a terminator.
	inline void WriteUtf8(s100::io::BinaryWriter* file, std::wstring_view text)
	{
		const std::string utf8 = s100::text::ToUtf8(text);
		file->Write(utf8.data(), utf8.size());
	}

	// Writes `text` as UTF-8 followed by the unit terminator (0x1F).
	inline void WriteUtf8Unit(s100::io::BinaryWriter* file, std::wstring_view text)
	{
		WriteUtf8(file, text);
		file->Write(&NonPrintableCharacter::unitTerminator, 1);
	}
}
