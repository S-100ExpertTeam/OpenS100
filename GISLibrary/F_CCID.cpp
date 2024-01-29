#include "stdafx.h"
#include "F_CCID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CCID::F_CCID(void)
{
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_rver = 0;
	m_ruin = 0;
}

F_CCID::F_CCID(const F_CCID& other)
{
	m_name = other.m_name;
	m_rver = other.m_rver;
	m_ruin = other.m_ruin;
}

F_CCID::F_CCID(RecordName recordName, int RVER, int RUIN)
{
	m_name = recordName;
	m_rver = RVER;
	m_ruin = RUIN;
}

F_CCID::~F_CCID(void)
{

}

void F_CCID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_rver = buf2uint(buf, 2);
	m_ruin = *(buf++);
}

bool F_CCID::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_rver, 2);
	file->Write(&m_ruin, 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_CCID::GetFieldLength()
{
	unsigned len = 0;
	len += m_name.GetLength();
	len += 2;
	len += 1;
	return ++len;
}

F_CCID* F_CCID::Clone() const
{
	F_CCID* f_ccid = new F_CCID();
	f_ccid->m_name = m_name;
	f_ccid->m_rver = m_rver;
	f_ccid->m_ruin = m_ruin;

	return f_ccid;
}

