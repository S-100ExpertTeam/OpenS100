#include "stdafx.h"
#include "F_C3IT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C3IT::F_C3IT()
{

}

F_C3IT::~F_C3IT()
{

}

int F_C3IT::GetSize()
{
	return 13;
}

void F_C3IT::ReadField(BYTE *&buf)
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
