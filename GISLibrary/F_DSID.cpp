#include "stdafx.h"
#include "F_DSID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_DSID::F_DSID(void)
{
	m_name.RCNM = 10;
	m_name.RCID = 1;
	m_ensp = "S-100 Part 10a";
	m_ened = "1.1";
	m_prsp = "INT.IHO.S-101.1.0";
	m_pred = "1.0";
	m_prof = "1";
	m_dsnm = ""; // 101KR005X01NE.000	
	m_dstl = ""; // S-101 Test Data Sets in ECDIS
	m_dsrd = "20231019"; // 20200626
	m_dslg = "EN";
	m_dsab = "";
	m_dsed = "1.0";
	m_dstc.Add(14);
	m_dstc.Add(18);
}

F_DSID::F_DSID(const F_DSID& other)
{
	m_name = other.m_name;
	m_ensp = other.m_ensp;
	m_ened = other.m_ened;
	m_prsp = other.m_prsp;
	m_pred = other.m_pred;
	m_prof = other.m_prof;
	m_dsnm = other.m_dsnm;
	m_dstl = other.m_dstl;
	m_dsrd = other.m_dsrd;
	m_dslg = other.m_dslg;
	m_dsab = other.m_dsab;
	m_dsed = other.m_dsed;
	for (int i = 0; i < other.m_dstc.GetSize(); i++)
		m_dstc.Add(other.m_dstc[i]);
}

F_DSID::~F_DSID(void)
{
	m_dstc.RemoveAll();

}

void F_DSID::ReadField(BYTE*& buf)
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

	m_dsrd.Empty();
	buf2charArr(m_dsrd, buf, 8);
	buf2charArr(m_dslg, buf);
	buf2charArr(m_dsab, buf);
	buf2charArr(m_dsed, buf);

	m_dstc.RemoveAll();
	while (*buf != 0x1E)
	{
		m_dstc.Add(*(buf++));
	}
}

bool F_DSID::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);

	CT2CA outputStringENSP(m_ensp, CP_UTF8);
	file->Write(outputStringENSP, (UINT)::strlen(outputStringENSP));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringENED(m_ened, CP_UTF8);
	file->Write(outputStringENED, (UINT)::strlen(outputStringENED));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringPRSP(m_prsp, CP_UTF8);
	file->Write(outputStringPRSP, (UINT)::strlen(outputStringPRSP));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringPRED(m_pred, CP_UTF8);
	file->Write(outputStringPRED, (UINT)::strlen(outputStringPRED));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringPROF(m_prof, CP_UTF8);
	file->Write(outputStringPROF, (UINT)::strlen(outputStringPROF));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringDSNM(m_dsnm, CP_UTF8);
	file->Write(outputStringDSNM, (UINT)::strlen(outputStringDSNM));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringDSTL(m_dstl, CP_UTF8);
	file->Write(outputStringDSTL, (UINT)::strlen(outputStringDSTL));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringDSRD(m_dsrd, CP_UTF8);
	file->Write(outputStringDSRD, (UINT)::strlen(outputStringDSRD));

	CT2CA outputStringDSLG(m_dslg, CP_UTF8);
	file->Write(outputStringDSLG, (UINT)::strlen(outputStringDSLG));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringDSAB(m_dsab, CP_UTF8);
	file->Write(outputStringDSAB, (UINT)::strlen(outputStringDSAB));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringDSED(m_dsed, CP_UTF8);
	file->Write(outputStringDSED, (UINT)::strlen(outputStringDSED));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	file->Write(&m_dstc[0], 1);
	file->Write(&m_dstc[1], 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_DSID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += m_ensp.GetLength() + 1;
	len += m_ened.GetLength() + 1;
	len += m_prsp.GetLength() + 1;
	len += m_pred.GetLength() + 1;
	len += m_prof.GetLength() + 1;
	len += m_dsnm.GetLength() + 1;
	len += m_dstl.GetLength() + 1;
	len += m_dsrd.GetLength();
	len += m_dslg.GetLength() + 1;
	len += m_dsab.GetLength() + 1;
	len += m_dsed.GetLength() + 1;
	len += (int)m_dstc.GetCount();
	return ++len;
}