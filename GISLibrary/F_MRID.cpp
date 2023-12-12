#include "stdafx.h"
#include "F_MRID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_MRID::F_MRID(void)
{
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_rver = 0;
	m_ruin = 0;
}

F_MRID::F_MRID(const F_MRID& other)
{
	m_name = other.m_name;
	m_rver = other.m_rver;
	m_ruin = other.m_ruin;
}

F_MRID::F_MRID(RecordName recordName, int RVER, int RUIN)
{
	m_name = recordName;
	m_rver = RVER;
	m_ruin = RUIN;
}

F_MRID::~F_MRID(void)
{

}

void F_MRID::ReadField(BYTE*& buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_rver = buf2uint(buf, 2);
	m_ruin = *(buf++);
}

bool F_MRID::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_rver, 2);
	file->Write(&m_ruin, 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}


int F_MRID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 2;
	len += 1;
	return ++len;
}