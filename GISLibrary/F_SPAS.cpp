#include "stdafx.h"
#include "F_SPAS.h"
#include "SPAS.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_SPAS::F_SPAS(void)
{

}

F_SPAS::F_SPAS(const F_SPAS& other) : Field(other)
{
	for (const auto& iter : other.m_arr)
	{
		SPAS* spas = new SPAS(*iter);
		m_arr.push_back(spas);
	}
}

F_SPAS::~F_SPAS(void)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		delete* itor;
	}
	m_arr.clear();
}

F_SPAS F_SPAS::operator=(const F_SPAS& other)
{
	for (auto& iter : m_arr)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	m_arr.clear();

	for (const auto& iter : other.m_arr)
	{
		SPAS* spas = new SPAS(*iter);
		m_arr.push_back(spas);
	}

	return *this;
}

void F_SPAS::ReadField(BYTE*& buf)
{
	while (*buf != 0x1E)
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

void F_SPAS::ReadField(BYTE*& buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
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

bool F_SPAS::WriteField(CFile* file)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_name.RCNM, 1);
		file->Write(&(*i)->m_name.RCID, 4);
		file->Write(&(*i)->m_ornt, 1);
		file->Write(&(*i)->m_smin, 4);
		file->Write(&(*i)->m_smax, 4);
		file->Write(&(*i)->m_saui, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_SPAS::GetFieldLength()
{
	int len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		SPAS* spas = *itor;
		len += SPAS::GetSize();
	}
	return ++len;
}

int F_SPAS::getCount() const
{
	return (int)m_arr.size();
}

SPAS* F_SPAS::getSPAS(int index) const
{
	if (index < 0 || index >= getCount()) {
		return nullptr;
	}

	return m_arr.at(index);
}

void F_SPAS::addSPAS(SPAS* item)
{
	m_arr.push_back(item);
}

void F_SPAS::addSPAS(RecordName recordName, int ornt, unsigned int smin, unsigned int smax, int saui)
{
	SPAS* item = new SPAS();
	item->Set(recordName, ornt, smin, smax, saui);
	m_arr.push_back(item);
}

