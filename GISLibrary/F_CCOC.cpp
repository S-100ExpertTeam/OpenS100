#include "stdafx.h"
#include "F_CCOC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CCOC::F_CCOC()
{
	m_ccui = 1;
	m_ccix = 0;
	m_ncco = 0;
}

F_CCOC::F_CCOC(const F_CCOC& other)
{
	m_ccui = other.m_ccui;
	m_ccix = other.m_ccix;
	m_ncco = other.m_ncco;
}

F_CCOC::~F_CCOC()
{

}

int F_CCOC::GetSize()
{
	return 5;
}

void F_CCOC::ReadField(BYTE*& buf)
{
	m_ccui = buf2uint(buf, 1);
	m_ccix = buf2uint(buf, 2);
	m_ncco = buf2uint(buf, 2);
}

int F_CCOC::GetFieldLength()
{
	int len = 0;
	len += 1;
	len += 2;
	len += 2;
	return ++len;
}

F_CCOC* F_CCOC::Clone() const
{
	F_CCOC* f_ccoc = new F_CCOC();
	f_ccoc->m_ccui = m_ccui;
	f_ccoc->m_ccix = m_ccix;
	f_ccoc->m_ncco = m_ncco;

	return f_ccoc;
}

