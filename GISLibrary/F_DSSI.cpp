#include "stdafx.h"
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
	m_cmfz = 100;
	m_noir = 0;
	m_nopn = 0;
	m_nomn = 0;
	m_nocn = 0;
	m_noxn = 0;
	m_nosn = 0;
	m_nofr = 0;
}

F_DSSI::F_DSSI(const F_DSSI& other)
{
	m_dcox = other.m_dcox;
	m_dcoy = other.m_dcoy;
	m_dcoz = other.m_dcoz;
	m_cmfx = other.m_cmfx;
	m_cmfy = other.m_cmfy;
	m_cmfz = other.m_cmfz;
	m_noir = other.m_noir;
	m_nopn = other.m_nopn;
	m_nomn = other.m_nomn;
	m_nocn = other.m_nocn;
	m_noxn = other.m_noxn;
	m_nosn = other.m_nosn;
	m_nofr = other.m_nofr;
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

bool F_DSSI::WriteField(CFile* file)
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

	return true;
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

int F_DSSI::GetNumberOfInformationTypeRecords()
{
	return m_noir;
}

void F_DSSI::SetNumberOfInformationTypeRecords(int value)
{
	m_noir = value;
}

int F_DSSI::GetNumberOfPointRecords()
{
	return m_nopn;
}

void F_DSSI::SetNumberOfPointRecords(int value)
{
	m_nopn = value;
}

int F_DSSI::GetNumberOfMultiPointRecords()
{
	return m_nomn;
}

void F_DSSI::SetNumberOfMultiPointRecords(int value)
{
	m_nomn = value;
}

int F_DSSI::GetNumberOfCurveRecords()
{
	return m_nocn;
}

void F_DSSI::SetNumberOfCurveRecords(int value)
{
	m_nocn = value;
}

int F_DSSI::GetNumberOfCompositeCurveRecords()
{
	return m_noxn;
}

void F_DSSI::SetNumberOfCompositeCurveRecords(int value)
{
	m_noxn = value;
}

int F_DSSI::GetNumberOfSurfaceRecords()
{
	return m_nosn;
}

void F_DSSI::SetNumberOfSurfaceRecords(int value)
{
	m_nosn = value;
}

int F_DSSI::GetNumberOfFeatureTypeRecords()
{
	return m_nofr;
}

void F_DSSI::SetNumberOfFeatureTypeRecords(int value)
{
	m_nofr = value;
}

F_DSSI* F_DSSI::Clone() const
{
	F_DSSI* f_dssi = new F_DSSI();

	f_dssi->m_dcox = m_dcox;
	f_dssi->m_dcoy = m_dcoy;
	f_dssi->m_dcoz = m_dcoz;
	f_dssi->m_cmfx = m_cmfx;
	f_dssi->m_cmfy = m_cmfy;
	f_dssi->m_cmfz = m_cmfz;
	f_dssi->m_noir = m_noir;
	f_dssi->m_nopn = m_nopn;
	f_dssi->m_nomn = m_nomn;
	f_dssi->m_nocn = m_nocn;
	f_dssi->m_noxn = m_noxn;
	f_dssi->m_nosn = m_nosn;
	f_dssi->m_nofr = m_nofr;

	return f_dssi;
}

