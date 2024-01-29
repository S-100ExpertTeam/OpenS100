#include "stdafx.h"
#include "F_C3IT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C3IT::F_C3IT()
{
	m_ycoo = 0;
	m_xcoo = 0;
	m_zcoo = 0;
}

F_C3IT::F_C3IT(const F_C3IT& other)
{
	m_vcid = other.m_vcid;
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_zcoo = other.m_zcoo;
}

F_C3IT::~F_C3IT()
{

}

int F_C3IT::GetSize()
{
	return 13;
}

void F_C3IT::ReadField(BYTE*& buf)
{
	m_vcid = *(buf++);
	m_ycoo = buf2int(buf, 4);
	m_xcoo = buf2int(buf, 4);
	m_zcoo = buf2int(buf, 4);
}

int F_C3IT::GetFieldLength()
{
	int len = 0;
	len += F_C3IT::GetSize();
	return ++len;
}

F_C3IT* F_C3IT::Clone() const
{
	F_C3IT* f_c3it = new F_C3IT();
	f_c3it->m_vcid = m_vcid;
	f_c3it->m_ycoo = m_ycoo;
	f_c3it->m_xcoo = m_xcoo;
	f_c3it->m_zcoo = m_zcoo;

	return f_c3it;
}
