#include "stdafx.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

#include <cmath>
#include <atlbase.h>
#include <atlconv.h>
#include <algorithm>
#include <bitset> // Convert the 32-bit binary encoding into hexadecimal

BYTE tBYTEArr[1024];
void buf2charArr(CString &dest, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0;
	int endLoc = 0;
	
	while (buf[endLoc] != NonPrintableCharacter::unitTerminator)
	{
		endLoc++;
	}
	for(i = 0; i < endLoc; i++)
	{
		tBYTEArr[i] = *(buf++);
	}
	tBYTEArr[i] = NULL;
	dest = CA2W((char*)tBYTEArr, CP_UTF8);
	buf++;
}

void buf2charArr(std::wstring &dest, BYTE*& buf)
{
	//USES_CONVERSION;
	int i = 0;
	int endLoc = 0;
	
	while(buf[endLoc] != 0x1F)
	{
		endLoc++;
	}
	for(i = 0; i < endLoc; i++)
	{
		tBYTEArr[i] = *(buf++);
	}
	tBYTEArr[i] = NULL;
	dest = ATL::CA2W((char*)tBYTEArr, CP_UTF8);
	buf++;
}

void buf2charArr(CString &dest, BYTE*& buf, int len)
{
	for(int i = 0; i < len; i++)
	{
		dest.AppendChar(*(buf++));
	}
}

void buf2charArr(std::wstring &dest, BYTE*& buf, int len)
{
//	USES_CONVERSION;
	int i = 0;
	for(i = 0; i < len; i++)
	{
		tBYTEArr[i] = *(buf++);
	}
	tBYTEArr[i] = NULL;
	dest = ATL::CA2W((char*)tBYTEArr, CP_UTF8);
}

DWORD buf2uint(BYTE*& buf, int size)
{
	DWORD ret = 0;
	memcpy_s(&ret, size, buf, size);

	buf += size;
	return ret;
}

DWORD buf2int(BYTE*& buf, int size)
{
	DWORD ret = 0;
	memcpy_s(&ret, size, buf, size);

	buf += size;
	return ret;
}

char TEMP_BUF256[1024];
DWORD atoi(BYTE*& buf, int len)
{
	DWORD ret = 0;
	for(int i=0; i< len;i++)
	{
		ret = ret * 10 + *(buf++) - '0';
	}
	return ret;
}

double buf2double(BYTE*& buf, int size)
{
	double ret = 0;
	memcpy_s(&ret, 8, buf, 8);
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