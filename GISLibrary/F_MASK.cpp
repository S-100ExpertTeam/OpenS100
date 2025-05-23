#include "stdafx.h"
#include "F_MASK.h"
#include "MASK.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_MASK::F_MASK(void)
{

}

F_MASK::F_MASK(const F_MASK& other) : Field(other)
{
	for (const auto& iter : other.listMask)
	{
		MASK* mask = new MASK(*iter);
		listMask.push_back(mask);
		m_arr.insert({ mask->m_name.GetName(), mask });
	}
}

F_MASK::~F_MASK(void)
{	
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		MASK* mask = i->second;
		delete mask;
	}
	listMask.clear();
	m_arr.clear();
}

F_MASK F_MASK::operator=(const F_MASK& other)
{
	for (auto& iter : listMask)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	listMask.clear();

	for (const auto& iter : other.listMask)
	{
		MASK* mask = new MASK(*iter);
		listMask.push_back(mask);
	}

	return *this;
}

void F_MASK::ReadField(BYTE*& buf)
{
	while (*buf != 0x1E)
	{
		MASK* mask = new MASK();
		mask->m_name.RCNM = *(buf++);
		mask->m_name.RCID = buf2uint(buf, 4);
		mask->m_mind = *(buf++);
		mask->m_muin = *(buf++);

		AddMask(mask);

	}
}

void F_MASK::ReadField(BYTE*& buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
	{
		MASK* mask = new MASK();
		mask->m_name.RCNM = *(buf++);
		mask->m_name.RCID = buf2uint(buf, 4);
		mask->m_mind = *(buf++);
		mask->m_muin = *(buf++);

		AddMask(mask);
	}
}

bool F_MASK::WriteField(CFile* file)
{
	for (auto i = listMask.begin(); i != listMask.end(); i++)
	{
		file->Write(&(*i)->m_name.RCNM, 1);
		file->Write(&(*i)->m_name.RCID, 4);
		file->Write(&(*i)->m_mind, 1);
		file->Write(&(*i)->m_muin, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}


int F_MASK::GetFieldLength()
{
	int len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		MASK* mask = itor->second;
		len += MASK::GetSize();
	}
	return ++len;
}

void F_MASK::AddMask(MASK* mask)
{
	listMask.push_back(mask);
	m_arr.insert({ mask->m_name.GetName(), mask });
}

void F_MASK::AddMask(RecordName recordName, int mind, int muin)
{
	MASK* mask = new MASK();
	mask->m_name = recordName;
	mask->m_mind = mind;
	mask->m_muin = muin;

	listMask.push_back(mask);
	m_arr.insert({ mask->m_name.GetName(), mask });
}

void F_MASK::DeleteMask(RecordName recordName)
{
	m_arr.erase(recordName.GetName());

	for (auto i = listMask.begin(); i != listMask.end(); i++) {
		if ((*i)->m_name == recordName) {
			listMask.erase(i);
			break;
		}
	}
}

MASK* F_MASK::GetMask(RecordName recordName)
{
	auto item = m_arr.find(recordName.GetName());
	if (item != m_arr.end()) {
		return item->second;
	}

	return nullptr;
}

int F_MASK::getCount() const
{
	return (int)m_arr.size();
}

MASK* F_MASK::getMASKbyIndex(int index) const
{
	if (index < 0 || index >= getCount()) {
		return nullptr;
	}

	return listMask.at(index);
}


