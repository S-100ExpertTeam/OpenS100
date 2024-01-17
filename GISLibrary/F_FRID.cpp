#include "stdafx.h"
#include "F_FRID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_FRID::F_FRID(void)
{
	m_name.RCNM = 100;
	m_name.RCID = 0;
}

F_FRID::F_FRID(const F_FRID& other) : Field(other)
{
	m_name = other.m_name;
	m_nftc = other.m_nftc;
	m_rver = other.m_rver;
	m_ruin = other.m_ruin;
}

F_FRID::F_FRID(RecordName recordName, int NFTC, int RVER, int RUIN)
{
	m_name = recordName;
	m_nftc = NFTC;
	m_rver = RVER;
	m_ruin = RUIN;
}

F_FRID::~F_FRID(void)
{

}

F_FRID F_FRID::operator=(const F_FRID& other)
{
	m_name = other.m_name;
	m_nftc = other.m_nftc;
	m_rver = other.m_rver;
	m_ruin = other.m_ruin;

	return *this;
}

void F_FRID::ReadField(BYTE*& buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_nftc = buf2uint(buf, 2);
	m_rver = buf2uint(buf, 2);
	m_ruin = *(buf++);
}

bool F_FRID::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_nftc, 2);
	file->Write(&m_rver, 2);
	file->Write(&m_ruin, 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_FRID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 2;
	len += 2;
	len += 1;
	return ++len;
}

