#include "stdafx.h"
#include "F_DSID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_DSID::F_DSID(void)
{
	m_name.RCNM = 10;
	m_name.RCID = 1;
	m_ensp = L"S-100 Part 10a";
	m_ened = L"1.1";
	m_prsp = L"INT.IHO.S-101.1.0";
	m_pred = L"1.0";
	m_prof = L"1";
	m_dsnm = L""; // 101KR005X01NE.000	
	m_dstl = L""; // S-101 Test Data Sets in ECDIS
	m_dsrd = L"20231019"; // 20200626
	m_dslg = L"EN";
	m_dsab = L"";
	m_dsed = L"1.0";
	m_dstc.push_back(14);
	m_dstc.push_back(18);
}

F_DSID::F_DSID(const F_DSID& other) : Field(other)
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
	m_dstc = other.m_dstc;
}

F_DSID::~F_DSID(void)
{
	m_dstc.clear();

}

F_DSID& F_DSID::operator=(const F_DSID& other)
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
	m_dstc = other.m_dstc;

	return *this;
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

	m_dsrd.clear();
	buf2charArr(m_dsrd, buf, 8);
	buf2charArr(m_dslg, buf);
	buf2charArr(m_dsab, buf);
	buf2charArr(m_dsed, buf);

	m_dstc.clear();
	while (*buf != 0x1E)
	{
		m_dstc.push_back(*(buf++));
	}
}

bool F_DSID::WriteField(s100::io::BinaryWriter* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);

	s100::iso8211::WriteUtf8Unit(file, m_ensp);
	s100::iso8211::WriteUtf8Unit(file, m_ened);
	s100::iso8211::WriteUtf8Unit(file, m_prsp);
	s100::iso8211::WriteUtf8Unit(file, m_pred);
	s100::iso8211::WriteUtf8Unit(file, m_prof);
	s100::iso8211::WriteUtf8Unit(file, m_dsnm);
	s100::iso8211::WriteUtf8Unit(file, m_dstl);
	s100::iso8211::WriteUtf8(file, m_dsrd);
	s100::iso8211::WriteUtf8Unit(file, m_dslg);
	s100::iso8211::WriteUtf8Unit(file, m_dsab);
	s100::iso8211::WriteUtf8Unit(file, m_dsed);
	file->Write(&m_dstc[0], 1);
	file->Write(&m_dstc[1], 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_DSID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += static_cast<int>(s100::iso8211::Utf8Length(m_ensp)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_ened)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_prsp)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_pred)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_prof)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dsnm)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dstl)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dsrd));
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dslg)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dsab)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dsed)) + 1;
	len += (int)m_dstc.size();
	return ++len;
}


