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

void CodeWithNumericCode::SetNumericCode(int value)
{
	this->m_nmcd = value;
}

std::wstring CodeWithNumericCode::GetCode()
{
	return std::wstring(m_code);
}

void CodeWithNumericCode::SetCode(std::wstring value)
{
	this->m_code = value.c_str();
}