#include "stdafx.h"
#include "CodeWithNumericCode.h"

CodeWithNumericCode::CodeWithNumericCode()
{

}

CodeWithNumericCode::CodeWithNumericCode(const CodeWithNumericCode& other)
{
	m_code = other.m_code;
	m_nmcd = other.m_nmcd;
}

CodeWithNumericCode::CodeWithNumericCode(CString& code, int numericCode)
{
	this->m_code = code;
	this->m_nmcd = numericCode;
}

CodeWithNumericCode CodeWithNumericCode::operator=(const CodeWithNumericCode& other)
{
	m_code = other.m_code;
	m_nmcd = other.m_nmcd;

	return *this;
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

std::string CodeWithNumericCode::getCodeAsString()
{
	return pugi::as_utf8(m_code);
}

void CodeWithNumericCode::SetCode(std::wstring value)
{
	this->m_code = value.c_str();
}