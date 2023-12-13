#include "stdafx.h"
#include "F_C2IT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2IT::F_C2IT()
{
}

F_C2IT::F_C2IT(const F_C2IT& other)
{
	m_xcoo = other.m_xcoo;
	m_ycoo = other.m_ycoo;
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

bool F_C2IT::WriteField(CFile* file)
{
	file->Write(&m_ycoo, 4);
	file->Write(&m_xcoo, 4);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_C2IT::GetFieldLength()
{
	int len = 0;
	len += F_C2IT::GetSize();

	return ++len;
}

F_C2IT* F_C2IT::Clone() const
{
	F_C2IT* f_c2it = new F_C2IT();
	f_c2it->m_ycoo = m_ycoo;
	f_c2it->m_xcoo = m_xcoo;

	return f_c2it;
}
