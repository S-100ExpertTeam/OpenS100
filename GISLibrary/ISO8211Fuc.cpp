#include "stdafx.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

#include <cmath>
#include <algorithm>
#include <bitset> // Convert the 32-bit binary encoding into hexadecimal

void buf2charArr(std::wstring& dest, std::uint8_t*& buf)
{
	std::size_t len = 0;
	while (buf[len] != NonPrintableCharacter::unitTerminator)
	{
		++len;
	}
	dest = s100::text::ToWide(std::string_view(reinterpret_cast<const char*>(buf), len));
	buf += len + 1; // skip the unit terminator
}

void buf2charArr(std::wstring& dest, std::uint8_t*& buf, int len)
{
	dest = s100::text::ToWide(std::string_view(reinterpret_cast<const char*>(buf), static_cast<std::size_t>(len)));
	buf += len;
}

std::uint32_t buf2uint(std::uint8_t*& buf, int size)
{
	std::uint32_t ret = 0;
	memcpy(&ret, buf, size);

	buf += size;
	return ret;
}

std::uint32_t buf2int(std::uint8_t*& buf, int size)
{
	std::uint32_t ret = 0;
	memcpy(&ret, buf, size);

	buf += size;
	return ret;
}

char TEMP_BUF256[1024];
std::uint32_t atoi(std::uint8_t*& buf, int len)
{
	std::uint32_t ret = 0;
	for(int i=0; i< len;i++)
	{
		ret = ret * 10 + *(buf++) - '0';
	}
	return ret;
}

double buf2double(std::uint8_t*& buf, int /*size*/)
{
	double ret = 0;
	memcpy(&ret, buf, 8);
	buf += 8;
	return ret;
}

int asNumeric( wchar_t wch )
{
	int result = -1;
	for ( wchar_t const* const* p = std::begin( digitTables );
		p != std::end( digitTables ) && result == -1;
		++ p ) {
			wchar_t const* q = std::find( *p, *p + 10, wch );
			if ( q != *p + 10 ) {
				result = (int)(q - *p);
			}
	}
	return result;
}