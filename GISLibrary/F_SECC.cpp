#include "stdafx.h"
#include "F_SECC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_SECC::F_SECC()
{
	m_seui = 1;
	m_seix = 0;
	m_nseg = 0;
}

F_SECC::F_SECC(const F_SECC& other)
{
	m_seui = other.m_seui;
	m_seix = other.m_seix;
	m_nseg = other.m_nseg;
}

F_SECC::~F_SECC()
{

}

F_SECC F_SECC::operator=(const F_SECC& other)
{
	m_seui = other.m_seui;
	m_seix = other.m_seix;
	m_nseg = other.m_nseg;

	return *this;
}

void F_SECC::ReadField(BYTE *&buf)
{
	m_seui = *(buf++);
	m_seix = buf2uint(buf, 2);
	m_nseg = buf2uint(buf, 2);
}

void F_SECC::ReadField(BYTE *&buf, int loopCnt)
{
	m_seui = *(buf++);
	m_seix = buf2uint(buf, 2);
	m_nseg = buf2uint(buf, 2);
}

int F_SECC::GetFieldLength()
{
	int len = 0;
	len += 1;
	len += 2;
	len += 2;
	return ++len;
}

