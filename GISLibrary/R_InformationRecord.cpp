#include "stdafx.h"
#include "R_InformationRecord.h"
#include "DRDirectoryInfo.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "NonPrintableCharacter.h"

#include "../LatLonUtility/LatLonUtility.h"

R_InformationRecord::R_InformationRecord(void)
{

}

R_InformationRecord::~R_InformationRecord(void)
{
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}
	m_inas.clear();
	for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	{
		delete *itor;
	}
	m_attr.clear();
}

BOOL R_InformationRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0, cnt;
	for(i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"IRID"))
		{
			m_irid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ATTR"))
		{
			cnt = 0;
			j = 0;
			BYTE *sIndex = buf;

			int head = 0;
			for (j = 1; j < dir->GetDirectory(i)->length; j++)
			{
				if (head == 0)
				{
					j += ATTR::GetOffsetToATVL() - 1;
					buf += ATTR::GetOffsetToATVL();
					head = 1;
				}
				else if (*(buf++) == 0x1F)
				{
					++cnt;
					head = 0;
				}
			}
			buf = sIndex;

			F_ATTR* attr = new F_ATTR();
			attr->ReadField(buf, cnt);
			m_attr.push_back(attr);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		
		if(*(buf++)!= 0x1E)//{}
			TRACE(W2A(TEXT("terminator error")));
	}
	return TRUE;
}

bool R_InformationRecord::WriteRecord(CFile* file)
{
	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_irid.GetFieldLength();
	Directory dir("IRID", fieldLength, fieldOffset);
	directory.push_back(dir);
	fieldOffset += fieldLength;

	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("ATTR", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("INAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	int totalFieldSize = fieldOffset;

	// Set leader
	SetLeader(totalFieldSize);
	leader.SetAsDR();
	leader.WriteLeader(file);

	// Write directory
	WriteDirectory(file);

	// Write field area
	m_irid.WriteField(file);

	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	return true;
}

int R_InformationRecord::GetRCID()
{
	return m_irid.m_name.RCID;
}

int R_InformationRecord::GetAssociationCount()
{
	return 0;
}

int R_InformationRecord::GetInformationAssociationCount()
{
	return (int)m_inas.size();
}

int R_InformationRecord::GetNumericCode()
{
	return m_irid.m_nitc;
}