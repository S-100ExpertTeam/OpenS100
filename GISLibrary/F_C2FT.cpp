#include "stdafx.h"
#include "F_C2FT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2FT::F_C2FT()
{
	m_xcoo = 0.0;
	m_ycoo = 0.0;
}

F_C2FT::F_C2FT(const F_C2FT& other)
{
	m_xcoo = other.m_xcoo;
	m_ycoo = other.m_ycoo;
}

F_C2FT::~F_C2FT()
{

}

int F_C2FT::GetSize()
{
	return 16;
}

F_C2FT F_C2FT::operator=(const F_C2FT& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;

	return *this;
}

void F_C2FT::ReadField(BYTE *&buf)
{
	m_ycoo = buf2double(buf, 8);
	m_xcoo = buf2double(buf, 8);
}

int  F_C2FT::GetFieldLength()
{
	unsigned len = 0;
	len += F_C2FT::GetSize();
	return ++len;
}
