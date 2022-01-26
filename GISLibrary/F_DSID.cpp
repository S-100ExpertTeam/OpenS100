#include "StdAfx.h"
#include "F_DSID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_DSID::F_DSID(void)
{
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_ensp = "";
	m_ened = "";
	m_prsp = "";
	m_pred = "";
	m_prof = "";
	m_dsnm = "";		
	m_dstl = "";
	m_dsrd = "";
	m_dslg = "";
	m_dsab = "";
	m_dsed = "";
}

F_DSID::~F_DSID(void)
{
	m_dstc.RemoveAll();

}

void F_DSID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	buf2charArr(m_ensp, buf);
	buf2charArr(m_ened, buf);
	buf2charArr(m_prsp, buf);
	buf2charArr(m_pred, buf);
	buf2charArr(m_prof, buf);
	buf2charArr(m_dsnm, buf);			
	buf2charArr(m_dstl, buf);
	buf2charArr(m_dsrd, buf, 8);
	buf2charArr(m_dslg, buf);
	buf2charArr(m_dsab, buf);
	buf2charArr(m_dsed, buf);
	while(*buf != 0x1E)
	{
		m_dstc.Add(*(buf++));
	}
}

int F_DSID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += m_ensp.GetLength()+1;
	len += m_ened.GetLength()+1;
	len += m_prsp.GetLength()+1;
	len += m_pred.GetLength()+1;
	len += m_prof.GetLength()+1;
	len += m_dsnm.GetLength()+1;
	len += m_dstl.GetLength()+1;
	len += m_dsrd.GetLength();
	len += m_dslg.GetLength()+1;
	len += m_dsab.GetLength()+1;
	len += m_dsed.GetLength()+1;
	len += (int)m_dstc.GetCount();
	return ++len;
}