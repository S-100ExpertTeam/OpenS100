#pragma once
#include <string>

void buf2charArr(CString& dest, BYTE*& buf);
void buf2charArr(std::wstring& dest, BYTE*& buf);
void buf2charArr(CString& dest, BYTE*& buf, int len);
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