#include "stdafx.h"
#include "F_C3FT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C3FT::F_C3FT()
{

}

F_C3FT::~F_C3FT()
{

}

int F_C3FT::GetSize()
{
	return 25;
}

void F_C3FT::ReadField(BYTE *&buf)
{
	m_ycoo = buf2double(buf, 8);
	m_xcoo = buf2double(buf, 8);
	m_zcoo = buf2double(buf, 8);
}

int F_C3FT::GetFieldLength()
{
	int len = 0;
	len += F_C3FT::GetSize();
	return ++len;
}
