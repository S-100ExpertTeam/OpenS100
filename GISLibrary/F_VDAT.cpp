#include "stdafx.h"
#include "F_VDAT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_VDAT::F_VDAT()
{
	m_dtnm = L"";
	m_dtid = L"";
	m_dtsr = 0;
	m_scri = L"";
}

F_VDAT::F_VDAT(const F_VDAT& other) : Field(other)
{
	m_dtnm = other.m_dtnm;
	m_dtid = other.m_dtid;
	m_dtsr = other.m_dtsr;
	m_scri = other.m_scri;
}

F_VDAT::~F_VDAT()
{

}

F_VDAT F_VDAT::operator=(const F_VDAT& other)
{
	m_dtnm = other.m_dtnm;
	m_dtid = other.m_dtid;
	m_dtsr = other.m_dtsr;
	m_scri = other.m_scri;

	return *this;
}

void F_VDAT::ReadField(BYTE*& buf)
{
	buf2charArr(m_dtnm, buf);
	buf2charArr(m_dtid, buf);
	m_dtsr = *(buf++);
	buf2charArr(m_scri, buf);
}

bool F_VDAT::WriteField(s100::io::BinaryWriter* file)
{
	s100::iso8211::WriteUtf8Unit(file, m_dtnm);
	s100::iso8211::WriteUtf8Unit(file, m_dtid);
	file->Write(&m_dtsr, 1);

	s100::iso8211::WriteUtf8Unit(file, m_scri);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_VDAT::GetFieldLength()
{
	int len = 0;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dtnm)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_dtid)) + 1;
	len++;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_scri)) + 1;
	return ++len;
}

