#include "stdafx.h"
#include "CodeWithNumericCode.h"

CodeWithNumericCode::CodeWithNumericCode()
{

}

CodeWithNumericCode::CodeWithNumericCode(CString& code, int numericCode)
{
	this->m_code = code;
	this->m_nmcd = numericCode;
}

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