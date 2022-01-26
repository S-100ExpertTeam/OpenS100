#include "stdafx.h"
#include "F_CCOC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CCOC::F_CCOC()
{

}

F_CCOC::~F_CCOC()
{

}

int F_CCOC::GetSize()
{
	return 5;
}

void F_CCOC::ReadField(BYTE *&buf)
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