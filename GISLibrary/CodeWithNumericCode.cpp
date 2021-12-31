#include "stdafx.h"
#include "CodeWithNumericCode.h"

int CodeWithNumericCode::GetSize()
{
	return 2;
}

int CodeWithNumericCode::GetNumericCode()
{
	return m_nmcd;
}

std::wstring CodeWithNumericCode::GetCode()
{
	return std::wstring(m_code);
}