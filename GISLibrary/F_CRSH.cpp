#include "stdafx.h"
#include "F_CRSH.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CRSH::F_CRSH(void)
{
	m_crix = 0;
	m_crst = 0;
	m_csty = 0;
	m_crnm = "";
	m_crsi = "";
	m_crss = 0;
	m_scri = "";
}

F_CRSH::~F_CRSH(void)
{

}

void F_CRSH::ReadField(BYTE *&buf)
{
	m_crix = *(buf++);
	m_crst = *(buf++);
	m_csty = *(buf++);
	buf2charArr(m_crnm, buf);
	buf2charArr(m_crsi, buf);
	m_crss = *(buf++);
	buf2charArr(m_scri, buf);
}

int F_CRSH::GetFieldLength()
{
	int len = 0;

	len += 1;
	len += 1;
	len += 1;
	len += m_crnm.GetLength() + 1;
	len += m_crsi.GetLength() + 1;
	len += 1;
	len += m_scri.GetLength() + 1;
	return ++len;
}