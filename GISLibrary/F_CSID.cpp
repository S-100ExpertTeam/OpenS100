#include "stdafx.h"
#include "F_CSID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CSID::F_CSID(void)
{
	m_name.RCNM = 15;
	m_name.RCID = 1;
	m_ncrc = 2;
}

F_CSID::F_CSID(const F_CSID& other) : Field(other)
{
	m_name = other.m_name;
	m_ncrc = other.m_ncrc;
}

F_CSID::~F_CSID(void)
{

}

F_CSID F_CSID::operator=(const F_CSID& other)
{
	m_name = other.m_name;
	m_ncrc = other.m_ncrc;

	return *this;
}

void F_CSID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_ncrc = *(buf++);
}

bool F_CSID::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_ncrc, 1);
	
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_CSID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 1;
	return ++len;
}

