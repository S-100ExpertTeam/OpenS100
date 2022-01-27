#include "StdAfx.h"
#include "F_SPAS.h"
#include "SPAS.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_SPAS::F_SPAS(void)
{

}

F_SPAS::~F_SPAS(void)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		delete *itor;
	}
}

void F_SPAS::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E)
	{
		SPAS* spas = new SPAS();
		spas->m_name.RCNM = *(buf++);
		spas->m_name.RCID = buf2uint(buf, 4);
		spas->m_ornt = *(buf++);
		spas->m_smin = buf2uint(buf, 4);
		spas->m_smax = buf2uint(buf, 4);
		spas->m_saui = *(buf++);

		m_arr.push_back(spas);
	}
}

void F_SPAS::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++)
	{
		SPAS* spas = new SPAS();
		spas->m_name.RCNM = *(buf++);
		spas->m_name.RCID = buf2uint(buf, 4);
		spas->m_ornt = *(buf++);
		spas->m_smin = buf2uint(buf, 4);
		spas->m_smax = buf2uint(buf, 4);
		spas->m_saui = *(buf++);

		m_arr.push_back(spas);
	}
}

int F_SPAS::GetFieldLength()
{
	int len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		SPAS *spas = *itor;
		len += SPAS::GetSize();
	}
	return ++len;
}