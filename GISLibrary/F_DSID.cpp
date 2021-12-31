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

BOOL F_DSID::Save(CFile *file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);

	CT2CA outputString(m_ensp, CP_UTF8);
	file->Write(outputString, (UINT)::strlen(outputString));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString2(m_ened, CP_UTF8);
	file->Write(outputString2, (UINT)::strlen(outputString2));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString3(m_prsp, CP_UTF8);
	file->Write(outputString3, (UINT)::strlen(outputString3));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString4(m_pred, CP_UTF8);
	file->Write(outputString4, (UINT)::strlen(outputString4));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString5(m_prof, CP_UTF8);
	file->Write(outputString5, (UINT)::strlen(outputString5));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString6(m_dsnm, CP_UTF8);
	file->Write(outputString6, (UINT)::strlen(outputString6));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString7(m_dstl, CP_UTF8);
	file->Write(outputString7, (UINT)::strlen(outputString7));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString8(m_dsrd, CP_UTF8);
	int size = (int)::strlen(outputString8);
	file->Write(outputString8, (UINT)::strlen(outputString8));

	CT2CA outputString9(m_dslg, CP_UTF8);
	file->Write(outputString9, (UINT)::strlen(outputString9));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString10(m_dsab, CP_UTF8);
	file->Write(outputString10, (UINT)::strlen(outputString10));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString11(m_dsed, CP_UTF8);
	file->Write(outputString11, (UINT)::strlen(outputString11));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);
	
	for(int i=0;i < m_dstc.GetCount();i++)
	{
		file->Write(&m_dstc[i], 1);
	}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
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