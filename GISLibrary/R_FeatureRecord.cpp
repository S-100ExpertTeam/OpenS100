#include "stdafx.h"
#include "R_FeatureRecord.h"
#include "F_SPAS.h"
#include "F_FASC.h"
#include "F_MASK.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "MASK.h"
#include "SPAS.h"
#include "DRDirectoryInfo.h"
#include "R_PointRecord.h"
#include "R_MultiPointRecord.h"
#include "R_CurveRecord.h"
#include "R_CompositeRecord.h"
#include "R_SurfaceRecord.h"

#include "SGeometry.h"
#include "SPoint.h"
#include "SSurface.h"

#include <pugixml.hpp>

R_FeatureRecord::R_FeatureRecord(void)
{
}

R_FeatureRecord::R_FeatureRecord(const R_FeatureRecord& other)
	: Record(other),
	GF::FeatureType(other)
{
	m_frid = other.m_frid;
	m_foid = other.m_foid;

	for (const auto& iter : other.m_attr)
	{
		F_ATTR* attr = iter;
		m_attr.push_back(attr);
	}

	for (const auto& iter : other.m_inas)
	{
		F_INAS* inas = iter;
		m_inas.push_back(inas);
	}

	for (const auto& iter : other.m_spas)
	{
		F_SPAS* spas = iter;
		m_spas.push_back(spas);
	}

	for (const auto& iter : other.m_fasc)
	{
		F_FASC* fasc = iter;
		m_fasc.push_back(fasc);
	}

	for (const auto& iter : other.m_mask)
	{
		F_MASK* mask = iter;
		m_mask.push_back(mask);
	}
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

	//if (m_geometry)
	//{
	//	m_geometry->Release();
	//	delete m_geometry;
	//	m_geometry = nullptr;
	//}
}

#pragma warning(disable:4018)
BOOL R_FeatureRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	unsigned i = 0, j = 0, cnt;
	for (i = 0; i < dir->m_count; i++)
	{
		if (strcmp(dir->GetDirectory(i)->tag, "FRID") == 0)
		{
			m_frid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "FOID") == 0)
		{
			m_foid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "ATTR") == 0)
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
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "SPAS") == 0)
		{
			F_SPAS* spas = new F_SPAS();
			cnt = (dir->GetDirectory(i)->length - 1) / SPAS::GetSize();
			spas->ReadField(buf, cnt);
			m_spas.push_back(spas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "FASC") == 0)
		{
			F_FASC *fasc = new F_FASC();
			cnt = (dir->GetDirectory(i)->length - 1) / ATTR::GetOffsetToATVL();
			fasc->ReadField(buf, cnt);
			m_fasc.push_back(fasc);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "MASK") == 0)
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

bool R_FeatureRecord::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_frid.GetFieldLength();
	Directory dirFRID("FRID", fieldLength, fieldOffset);
	directory.push_back(dirFRID);
	fieldOffset += fieldLength;

	fieldLength = m_foid.GetFieldLength();
	Directory dirFOID("FOID", fieldLength, fieldOffset);
	directory.push_back(dirFOID);
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

	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("SPAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_fasc.begin(); i != m_fasc.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("FASC", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_mask.begin(); i != m_mask.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("MASK", fieldLength, fieldOffset);
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
	m_frid.WriteField(file);
	m_foid.WriteField(file);

	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_fasc.begin(); i != m_fasc.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_mask.begin(); i != m_mask.end(); i++)
	{
		(*i)->WriteField(file);
	}

	return true;
}

RecordName R_FeatureRecord::GetRecordName()
{
	return m_frid.m_name;
}

void R_FeatureRecord::Draw(HDC &hdc, Scaler *scaler, double offset)
{
	if (geometry != NULL)
	{
		geometry->DrawGeometry(hdc, scaler, offset);
		geometry->DrawTextInfo(hdc, scaler, offset);
	}
}

MBR R_FeatureRecord::GetMBR()
{
	if (!geometry)
		return MBR(0, 0, 0, 0);

	return geometry->m_mbr;
}

int R_FeatureRecord::GetRCID()
{
	return m_frid.m_name.RCID;
}

std::string R_FeatureRecord::GetRCIDAsString(std::string prefix)
{
	return prefix + std::to_string(GetRCID());
}

void R_FeatureRecord::SetRCID(int rcid)
{
	m_frid.m_name.RCID = rcid;
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

void R_FeatureRecord::SetNumericCode(int numericCode)
{
	m_frid.m_nftc = numericCode;
}

//int R_FeatureRecord::MinimumDisplayPriority()
//{
//	return minimumDisplayPriority;
//}
//
//void R_FeatureRecord::MinimumDisplayPriority(int value)
//{
//	minimumDisplayPriority = value;
//}
//
//int R_FeatureRecord::MaximumDisplayPriority()
//{
//	return maximumDisplayPriority;
//}
//
//void R_FeatureRecord::MaximumDisplayPriority(int value)
//{
//	maximumDisplayPriority = value;
//}

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

std::vector<ATTR*> R_FeatureRecord::GetAllAttributes()
{
	if (m_attr.size() == 0)
	{
		return std::vector<ATTR*>();
	}

	return m_attr.front()->m_arr;
}

std::vector<ATTR*> R_FeatureRecord::GetRootAttributes()
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

std::vector<ATTR*> R_FeatureRecord::GetRootAttributes(int numericCode)
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

std::vector<ATTR*> R_FeatureRecord::GetChildAttributes(ATTR* parentATTR)
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

std::vector<ATTR*> R_FeatureRecord::GetChildAttributes(ATTR* parentATTR, int numericCode)
{
	auto allAttributes = GetAllAttributes();

	std::vector<ATTR*> result;
	int parentIndex = -1;

	for (int i = 0; i < allAttributes.size(); i++)
	{
		if (parentATTR == allAttributes[i])
		{
			parentIndex = i + 1;
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

std::vector<F_INAS*> R_FeatureRecord::GetAllInformationAssociations()
{
	std::vector<F_INAS*> result;
	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		result.push_back(*i);
	}

	return result;
}

std::vector<F_FASC*> R_FeatureRecord::GetAllFeatureAssociations()
{
	std::vector<F_FASC*> result;
	for (auto i = m_fasc.begin(); i != m_fasc.end(); i++)
	{
		result.push_back(*i);
	}

	return result;
}

int R_FeatureRecord::GetAttributeIndex(ATTR* attr)
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

SPAS* R_FeatureRecord::CreateEmptySPAS()
{
	if (m_spas.size() == 0)
	{
		m_spas.push_back(new F_SPAS());
	}

	if (m_spas.front()->m_arr.size() == 0)
	{
		m_spas.front()->m_arr.push_back(new SPAS());
	}

	return GetSPAS();
}

void R_FeatureRecord::SetSPAS(int rcnm, int rcid, int ornt)
{
	auto spas = CreateEmptySPAS();
	if (spas)
	{
		spas->m_name.RCNM = rcnm;
		spas->m_name.RCID = rcid;
		spas->m_ornt = ornt;
	}
}

void R_FeatureRecord::SetVectorRecord(R_VectorRecord* record)
{
	auto spas = GetSPAS();
	if (nullptr == spas)
	{
		spas = CreateEmptySPAS();
	}

	if (spas)
	{
		spas->Set(record->GetRecordName());
	}
}

std::vector<MASK*> R_FeatureRecord::GetAllMASK()
{
	std::vector<MASK*> result;

	for (auto i = m_mask.begin(); i != m_mask.end(); i++) {
		auto mask = *i;
		for (auto j = mask->listMask.begin(); j != mask->listMask.end(); j++) {
			result.push_back(*j);
		}
	}

	return result;
}

R_FeatureRecord* R_FeatureRecord::Clone() const
{
	R_FeatureRecord* fr = new R_FeatureRecord();
	fr->m_frid = m_frid;
	fr->m_foid = m_foid;

	for (const auto& iter : m_attr)
	{
		F_ATTR* f_attr = (!iter) ? nullptr : iter->Clone();
		fr->m_attr.push_back(f_attr);
	}

	for (const auto& iter : m_inas)
	{
		F_INAS* f_inas = (!iter) ? nullptr : iter->Clone();
		fr->m_inas.push_back(f_inas);
	}

	for (const auto& iter : m_spas)
	{
		F_SPAS* f_spas = (!iter) ? nullptr : iter->Clone();
		fr->m_spas.push_back(f_spas);
	}

	for (const auto& iter : m_fasc)
	{
		F_FASC* f_fasc = (!iter) ? nullptr : iter->Clone();
		fr->m_fasc.push_back(f_fasc);
	}

	for (const auto& iter : m_mask)
	{
		F_MASK* f_mask = (!iter) ? nullptr : iter->Clone();
		fr->m_mask.push_back(f_mask);
	}
}

std::string R_FeatureRecord::GetID()
{
	return std::to_string(GetRCID());
}

std::wstring R_FeatureRecord::GetIDAsWString()
{
	return std::to_wstring(GetRCID());
}

int R_FeatureRecord::GetIDAsInteger()
{
	return GetRCID();
}

bool R_FeatureRecord::IsNoGeometry()
{
	return GetSPASCount() == 0;
}

int R_FeatureRecord::GetFeatureRelationCount()
{
	return (int)m_fasc.size();
}

std::string R_FeatureRecord::GetAssociatedFeatureID(int index)
{
	if (index >= 0 && index < GetFeatureRelationCount())
	{
		auto i = m_fasc.begin();
		std::advance(i, index);
		return (*i)->m_name.GetRCIDasString();
	}

	return "";
}

std::string R_FeatureRecord::GetAssociatedInformationID(int index)
{
	if (index >= 0 && index < GetInformationRelationCount())
	{
		auto i = m_inas.begin();
		std::advance(i, index);
		return (*i)->m_name.GetRCIDasString();
	}

	return "";
}

SpatialPrimitiveType R_FeatureRecord::GetSpatialPrimitiveType()
{
	auto rcnm = GetSPASRCNM();
	if (rcnm == 0)
	{
		return SpatialPrimitiveType::noGeometry;
	}
	else if (rcnm == 110)
	{
		return SpatialPrimitiveType::point;
	}
	else if (rcnm == 115)
	{
		return SpatialPrimitiveType::pointSet;
	}
	else if (rcnm == 120 || rcnm == 125)
	{
		return SpatialPrimitiveType::curve;
	}
	else if (rcnm == 130)
	{
		return SpatialPrimitiveType::surface;
	}

	return SpatialPrimitiveType::none;
}

GM::Object* R_FeatureRecord::GetGMGeometry()
{
	return nullptr;
}

int R_FeatureRecord::GetInformationRelationCount()
{
	return (int)m_inas.size();
}

int R_FeatureRecord::GetAttributeCount() const
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



std::string R_FeatureRecord::GetAttributeValue(int index)
{
	auto attributes = GetAllAttributes();
	int count = (int)attributes.size();

	if (count > 0 && index < count)
	{
		return pugi::as_utf8(std::wstring(attributes.at(index)->m_atvl));
	}

	return "";
}

int R_FeatureRecord::GetParentAttributeIndex(int index)
{
	auto attr = GetAllAttributes();
	if (index >= 0 && index < attr.size())
	{
		return attr.at(index)->m_paix;
	}

	return 0;
}