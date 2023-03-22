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
		if(strcmp(dir->GetDirectory(i)->tag, "IRID") == 0)
		{
			m_irid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "ATTR") == 0)
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
	directory.clear();

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

RecordName R_InformationRecord::GetRecordName()
{
	return m_irid.m_name;
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
	return m_irid.NITC();
}


std::vector<ATTR*> R_InformationRecord::GetAllAttributes()
{
	if (m_attr.size() == 0)
	{
		return std::vector<ATTR*>();
	}

	return m_attr.front()->m_arr;
}

std::vector<ATTR*> R_InformationRecord::GetRootAttributes()
{
	auto allAttributes = GetAllAttributes();

	std::vector<ATTR*> result;

	for (auto i = allAttributes.begin(); i != allAttributes.end(); i++)
	{
		if ((*i)->m_paix == 0)
		{
			result.push_back(*i);
		}
	}

	return result;
}

std::vector<ATTR*> R_InformationRecord::GetRootAttributes(int numericCode)
{
	auto allAttributes = GetAllAttributes();

	std::vector<ATTR*> result;

	for (auto i = allAttributes.begin(); i != allAttributes.end(); i++)
	{
		if ((*i)->m_paix == 0 && (*i)->m_natc == numericCode)
		{
			result.push_back(*i);
		}
	}

	return result;
}

std::vector<ATTR*> R_InformationRecord::GetChildAttributes(ATTR* parentATTR)
{
	auto allAttributes = GetAllAttributes();

	std::vector<ATTR*> result;
	int parentIndex = -1;

	for (int i = 0; i < allAttributes.size(); i++)
	{
		if (parentATTR == allAttributes[i])
		{
			parentIndex = i;
			break;
		}
	}

	if (parentIndex > 0)
	{
		for (auto i = allAttributes.begin(); i != allAttributes.end(); i++)
		{
			if ((*i)->m_paix == parentIndex)
			{
				result.push_back(*i);
			}
		}
	}

	return result;
}

std::vector<ATTR*> R_InformationRecord::GetChildAttributes(ATTR* parentATTR, int numericCode)
{
	auto allAttributes = GetAllAttributes();

	std::vector<ATTR*> result;
	int parentIndex = -1;

	for (int i = 0; i < allAttributes.size(); i++)
	{
		if (parentATTR == allAttributes[i])
		{
			parentIndex = i;
			break;
		}
	}

	if (parentIndex > 0)
	{
		for (auto i = allAttributes.begin(); i != allAttributes.end(); i++)
		{
			if ((*i)->m_paix == parentIndex && (*i)->m_natc == numericCode)
			{
				result.push_back(*i);
			}
		}
	}

	return result;
}

int R_InformationRecord::GetAttributeIndex(ATTR* attr)
{
	if (m_attr.size() > 0)
	{
		auto ATTRs = m_attr.front();
		for (
			int i = 0;
			i < ATTRs->m_arr.size();
			i++)
		{
			if (ATTRs->m_arr[i] == attr)
			{
				return i + 1;
			}
		}
	}

	return 0;
}

std::string R_InformationRecord::GetID()
{
	return std::to_string(GetRCID());
}

std::wstring R_InformationRecord::GetIDAsWString()
{
	return std::to_wstring(GetRCID());
}

int R_InformationRecord::GetIDAsInteger()
{
	return GetRCID();
}

int R_InformationRecord::GetInformationRelationCount()
{
	return m_inas.size();
}

std::string R_InformationRecord::GetAssociatedInformationID(int index)
{
	if (index >= 0 && index < GetInformationRelationCount())
	{
		auto i = m_inas.begin();
		std::advance(i, index);
		return (*i)->m_name.GetRCIDasString();
	}

	return "";
}

int R_InformationRecord::GetAttributeCount()
{
	int size = 0;

	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		auto attr = *i;
		for (auto j = attr->m_arr.begin(); j != attr->m_arr.end(); j++)
		{
			size++;
		}
	}

	return size;
}