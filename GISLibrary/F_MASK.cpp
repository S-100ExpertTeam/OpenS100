#include "StdAfx.h"
#include "F_MASK.h"
#include "MASK.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_MASK::F_MASK(void)
{

}

F_MASK::~F_MASK(void)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		MASK *mask = itor->second;
		delete mask;
	}
}

void F_MASK::ReadField(BYTE *&buf)
{
	while (*buf != 0x1E)
	{
		MASK *mask = new MASK();
		mask->m_name.RCNM = *(buf++);
		mask->m_name.RCID = buf2uint(buf, 4);
		mask->m_mind = *(buf++);
		mask->m_muin = *(buf++);

		m_arr.insert(
			std::unordered_map<__int64, MASK*>::value_type((__int64)mask->m_name.GetName(), mask)
			);
	}
}

void F_MASK::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++)
	{
		MASK *mask = new MASK();
		mask->m_name.RCNM = *(buf++);
		mask->m_name.RCID = buf2uint(buf, 4);
		mask->m_mind = *(buf++);
		mask->m_muin = *(buf++);
		
		m_arr.insert(
			std::unordered_map<__int64, MASK*>::value_type((__int64)mask->m_name.GetName(), mask)
			);
	}
}

BOOL F_MASK::Save(CFile *file)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		MASK *mask = itor->second;
		
		file->Write(&mask->m_name.RCNM, 1);
		file->Write(&mask->m_name.RCID, 4);
		file->Write(&mask->m_mind, 1);
		file->Write(&mask->m_muin, 1);
	}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_MASK::GetFieldLength()
{
	int len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		MASK *mask = itor->second;
		len += MASK::GetSize();
	}
	return ++len;
}