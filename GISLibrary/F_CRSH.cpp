#include "stdafx.h"
#include "F_CRSH.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CRSH::F_CRSH(void)
{
	m_crix = 0;
	m_crst = 0;
	m_csty = 0;
	m_crnm = L"";
	m_crsi = L"";
	m_crss = 0;
	m_scri = L"";
}

F_CRSH::F_CRSH(const F_CRSH& other) : Field(other)
{
	m_crix = other.m_crix;
	m_crst = other.m_crst;
	m_csty = other.m_csty;
	m_crnm = other.m_crnm;
	m_crsi = other.m_crsi;
	m_crss = other.m_crss;
	m_scri = other.m_scri;
}

F_CRSH::~F_CRSH(void)
{

}

F_CRSH F_CRSH::operator=(const F_CRSH& other)
{
	m_crix = other.m_crix;
	m_crst = other.m_crst;
	m_csty = other.m_csty;
	m_crnm = other.m_crnm;
	m_crsi = other.m_crsi;
	m_crss = other.m_crss;
	m_scri = other.m_scri;

	return *this;
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

bool F_CRSH::WriteField(s100::io::BinaryWriter* file)
{
	file->Write(&m_crix, 1);
	file->Write(&m_crst, 1);
	file->Write(&m_csty, 1);

	s100::iso8211::WriteUtf8Unit(file, m_crnm);
	s100::iso8211::WriteUtf8Unit(file, m_crsi);
	file->Write(&m_crss, 1);
	
	s100::iso8211::WriteUtf8Unit(file, m_scri);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_CRSH::GetFieldLength()
{
	int len = 0;

	len += 1;
	len += 1;
	len += 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_crnm)) + 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_crsi)) + 1;
	len += 1;
	len += static_cast<int>(s100::iso8211::Utf8Length(m_scri)) + 1;
	return ++len;
}


