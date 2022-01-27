#include "stdafx.h"
#include "F_C2IT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2IT::F_C2IT()
{
	m_ycoo = 0;
	m_xcoo = 0;
}

F_C2IT::~F_C2IT()
{

}

int F_C2IT::GetSize()
{
	return 8;
}

void F_C2IT::ReadField(BYTE *&buf)
{
	m_ycoo = buf2int(buf, 4);
	m_xcoo = buf2int(buf, 4);
}

int F_C2IT::GetFieldLength()
{
	int len = 0;
	len += F_C2IT::GetSize();

	return ++len;
}