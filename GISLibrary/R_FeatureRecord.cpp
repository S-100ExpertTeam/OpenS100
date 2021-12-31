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
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SSurface.h"
#include "..\\S100Geometry\\SCurveHasOrient.h"
#include "..\\S100Geometry\\SCurve.h"

R_FeatureRecord::R_FeatureRecord(void)
{
	m_geometry = NULL;
	m_scaleMin = 0;
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

void R_FeatureRecord::Draw(CDC *pDC, Scaler *scaler, double offset)
{
	if (m_geometry != NULL)
	{
	}
}

void R_FeatureRecord::Draw(HDC &hdc, Scaler *scaler, double offset)
{
	if (m_geometry != NULL)
	{
		m_geometry->DrawGeometry(hdc, scaler, offset);
		m_geometry->DrawTextInfo(hdc, scaler, offset);
	}
}

void R_FeatureRecord::Draw(HDC &hDC, Scaler *scaler, int type, int priority, double offset)
{

}

void R_FeatureRecord::CreateCS(S101Cell *cell, CString csName, bool bSENC)
{
	if (csName.Find(L"L") >= 0)
		int io = 0;

#ifdef _DEBUG11
	return;
#endif
	return;
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

void R_FeatureRecord::GetXYPointOfTheVisiblePoints(double &x, double &y)
{
	std::list<SCurveHasOrient> *pListCurveLink = NULL;
	if (m_geometry->type == 1)
	{
		SPoint* p = (SPoint*)m_geometry;
		x = p->m_point.GetX();
		y = p->m_point.GetY();

		return;
	}
	else if (m_geometry->type == 2)
	{
		SCompositeCurve* cc = (SCompositeCurve*)m_geometry;

		pListCurveLink = &cc->m_listCurveLink;
	}
	else if (m_geometry->type == 3)
	{
		SSurface* s = (SSurface*)m_geometry;

		pListCurveLink = &s->m_listCurveLink;
	}

	if (pListCurveLink)
	{
		for (auto i = pListCurveLink->begin(); i != pListCurveLink->end(); i++)
		{
			SCurve* curve = (*i).GetCurve();
			if (!(*i).GetMasking())
			{
				if (curve->m_numPoints > 2)
				{
					x = curve->m_pPoints[1].GetX();
					y = curve->m_pPoints[1].GetY();

					return;
				}
			}
		}

		if (m_geometry->type == 3)
		{
			SSurface* s = (SSurface*)m_geometry;

			if (s->m_centerPoint)
			{
				x = s->m_centerPoint->GetX();
				y = s->m_centerPoint->GetY();
				return;
			}
		}
	}
	return;
}


int R_FeatureRecord::GetAttributeIndex(ATTR* value)
{
	int result = 1;
	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		auto fATTR = *i;
		if (fATTR == nullptr)
		{
			return -1;
		}
		if (fATTR != nullptr)
		{
			for (auto j = fATTR->m_arr.begin(); j != fATTR->m_arr.end(); j++)
			{
				auto attr = *j;
				if (attr == value)
				{
					return result;
				}
				else
				{
					result++;
				}
			}
		}
	}
	return result;
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

SGeometry* R_FeatureRecord::GetGeometry()
{
	return m_geometry;
}