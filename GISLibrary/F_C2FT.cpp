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

F_C2FT* F_C2FT::Clone() const
{
	F_C2FT* f_c2ft = new F_C2FT();
	f_c2ft->m_ycoo = m_ycoo;
	f_c2ft->m_xcoo = m_xcoo;

	return f_c2ft;
}
