#include "StdAfx.h"
#include "R_FeatureRecord.h"
#include "F_SPAS.h"
#include "F_FASC.h"
#include "F_MASK.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "FASC.h"
#include "MASK.h"
#include "SPAS.h"
#include "DRDirectoryInfo.h"

#include "..\\S100Geometry\\SGeometry.h"
#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SSurface.h"
#include "..\\S100Geometry\\SCurve.h"

R_FeatureRecord::R_FeatureRecord(void)
{
}

R_FeatureRecord::~R_FeatureRecord(void)
{
	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_attr.clear();

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_inas.clear();

	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_spas.clear();

	for (auto i = m_fasc.begin(); i != m_fasc.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_fasc.clear();

	for (auto i = m_mask.begin(); i != m_mask.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_mask.clear();

	delete m_geometry;
	m_geometry = nullptr;

	//	[Text Placement ]
	delete m_textBearing;
	m_textBearing = nullptr;
}


#pragma warning(disable:4018)
BOOL R_FeatureRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	unsigned i = 0, j = 0, cnt;
	for (i = 0; i < dir->m_count; i++)
	{
		if (dir->GetDirectory(i)->tag == *((unsigned int*)"FRID"))
		{
			m_frid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"FOID"))
		{
			m_foid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ATTR"))
		{
			cnt = 0;
			j = 0;
			BYTE *sIndex = buf;

			unsigned head = 0;
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
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"SPAS"))
		{
			F_SPAS* spas = new F_SPAS();
			cnt = (dir->GetDirectory(i)->length - 1) / SPAS::GetSize();
			spas->ReadField(buf, cnt);
			m_spas.push_back(spas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"FASC"))
		{
			F_FASC *fasc = new F_FASC();
			cnt = (dir->GetDirectory(i)->length - 1) / FASC::GetSize();
			fasc->ReadField(buf, cnt);
			m_fasc.push_back(fasc);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"MASK"))
		{

			F_MASK* mask = new F_MASK();
			cnt = (dir->GetDirectory(i)->length - 1) / MASK::GetSize();
			mask->ReadField(buf, cnt);
			m_mask.push_back(mask);

		}
		else
		{
			buf += dir->GetDirectory(i)->length;

		}

		if (*(buf++) != 0x1E)
		{
		}

	}
	return true;
}

void R_FeatureRecord::Draw(HDC &hdc, Scaler *scaler, double offset)
{
	if (m_geometry != NULL)
	{
		m_geometry->DrawGeometry(hdc, scaler, offset);
		m_geometry->DrawTextInfo(hdc, scaler, offset);
	}
}

MBR R_FeatureRecord::GetMBR()
{
	if (!m_geometry)
		return MBR(0, 0, 0, 0);

	return m_geometry->m_mbr;
}

int R_FeatureRecord::GetRCID()
{
	return m_frid.m_name.RCID;
}

int R_FeatureRecord::GetAssociationCount()
{
	return GetFeatureAssociationCount() + GetInformationAssociationCount();
}

int R_FeatureRecord::GetFeatureAssociationCount()
{
	return (int)m_fasc.size();
}

int R_FeatureRecord::GetInformationAssociationCount()
{
	return (int)m_inas.size();
}

int R_FeatureRecord::GetNumericCode()
{
	return m_frid.m_nftc;
}

int R_FeatureRecord::MinimumDisplayPriority()
{
	return minimumDisplayPriority;
}

void R_FeatureRecord::MinimumDisplayPriority(int value)
{
	minimumDisplayPriority = value;
}

int R_FeatureRecord::MaximumDisplayPriority()
{
	return maximumDisplayPriority;
}

void R_FeatureRecord::MaximumDisplayPriority(int value)
{
	maximumDisplayPriority = value;
}

int R_FeatureRecord::GetSPASRCNM()
{
	if (m_spas.size() == 0)
	{
		return 0;
	}

	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		F_SPAS *spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			SPAS* spas = *j;

			auto rcnm = spas->m_name.RCNM;
			if (rcnm == 110 || rcnm == 115 || rcnm == 120 || rcnm == 125 || rcnm == 130)
			{
				return rcnm;
			}
			else
			{
				return -1;
			}
		}
	}

	return -1;
}

int R_FeatureRecord::GetSPASCount()
{
	int result = 0;

	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		F_SPAS* spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			result++;
		}
	}

	return result;
}

SGeometry* R_FeatureRecord::GetGeometry()
{
	return m_geometry;
}

SPAS* R_FeatureRecord::GetSPAS()
{
	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		F_SPAS* spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			return spasParent->m_arr.front();
		}
	}

	return nullptr;
}