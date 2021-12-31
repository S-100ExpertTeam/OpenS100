#include "StdAfx.h"
#include "F_DSSI.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_DSSI::F_DSSI(void)
{
	m_dcox = 0;
	m_dcoy = 0;
	m_dcoz = 0;
	m_cmfx = 10000000;
	m_cmfy = 10000000;
	m_cmfz = 10000000;
	m_noir = 0;
	m_nopn = 0;
	m_nomn = 0;
	m_nocn = 0;
	m_noxn = 0;
	m_nosn = 0;
	m_nofr = 0;
}

F_DSSI::~F_DSSI(void)
{

}

void F_DSSI::ReadField(BYTE *&buf)
{
	m_dcox = buf2double(buf, 8);
	m_dcoy = buf2double(buf, 8);
	m_dcoz = buf2double(buf, 8);
	m_cmfx = buf2uint(buf, 4);
	m_cmfy = buf2uint(buf, 4);
	m_cmfz = buf2uint(buf, 4);
	m_noir = buf2uint(buf, 4);
	m_nopn = buf2uint(buf, 4);
	m_nomn = buf2uint(buf, 4);
	m_nocn = buf2uint(buf, 4);
	m_noxn = buf2uint(buf, 4);
	m_nosn = buf2uint(buf, 4);
	m_nofr = buf2uint(buf, 4);
}

BOOL F_DSSI::Save(CFile *file)
{
	file->Write(&m_dcox, 8);
	file->Write(&m_dcoy, 8);
	file->Write(&m_dcoz, 8);
	file->Write(&m_cmfx, 4);
	file->Write(&m_cmfy, 4);
	file->Write(&m_cmfz, 4);
	file->Write(&m_noir, 4);
	file->Write(&m_nopn, 4);
	file->Write(&m_nomn, 4);
	file->Write(&m_nocn, 4);
	file->Write(&m_noxn, 4);
	file->Write(&m_nosn, 4);
	file->Write(&m_nofr, 4);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_DSSI::GetFieldLength()
{
	int len = 0;
	len += 8;
	len += 8;
	len += 8;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	len += 4;
	return ++len;
}