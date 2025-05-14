#include "stdafx.h"
#include "S101Cell.h"
#include "GISLibrary.h"
#include "S100Layer.h"
#include "F_CUCO.h"
#include "F_PTAS.h"
#include "F_SPAS.h"
#include "F_MASK.h"
#include "F_FASC.h"
#include "F_C3IL.h"
#include "F_C2IL.h"
#include "F_C3IT.h"
#include "F_C2IT.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "F_RIAS.h"
#include "R_CompositeRecord.h"
#include "R_SurfaceRecord.h"
#include "R_InformationRecord.h"
#include "R_MultiPointRecord.h"
#include "R_CurveRecord.h"
#include "R_FeatureRecord.h"
#include "R_PointRecord.h"
#include "D2D1Resources.h"
#include "ATTR.h"
#include "C3IL.h"
#include "C2IL.h"
#include "CUCO.h"
#include "F_COCC.h"
#include "CodeWithNumericCode.h"
#include "SPAS.h"
#include "PTAS.h"
#include "RIAS.h"
#include "ISO8211Fuc.h"
#include "F_C2IL.h"
#include "F_C3IL.h"
#include "F_CCOC.h"
#include "F_CCOC.h"
#include "MASK.h"
#include "F_SECC.h"
#include "R_DDR.h"
#include "DRDirectoryInfo.h"
#include "PCOutputSchemaManager.h"
#include "SENC_SpatialReference.h"
#include "SENC_Attribute.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_PointInstruction.h"
#include "Record.h"
#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "SCommonFuction.h"
#include "SCurve.h"
#include "SCurve.h"
#include "SGeometricFuc.h"
#include "GML_Envelop.h"
#include "S10XGML.h"
#include "GM_Point.h"
#include "GM_MultiPoint.h"
#include "GM_Curve.h"
#include "GM_OrientableCurve.h"
#include "GM_CompositeCurve.h"
#include "GM_Surface.h"
#include "S101Creator.h"

#include "../FeatureCatalog/FeatureCatalogue.h"
#include "../FeatureCatalog/S100_CD_AttributeValueType.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <sstream> 
#include <iomanip>
#include <mmsystem.h> 
#include <unordered_map>
#include <vector>
#include <string>


S101Cell::S101Cell(D2D1Resources* d2d1) : S100SpatialObject(d2d1)
{
	type = S100SpatialObjectType::S101Cell;
	m_FileType = S100_FileType::FILE_S_100_VECTOR;
	m_ObejctType = SpatialObjectType::S101Cell;
}

S101Cell::S101Cell(FeatureCatalogue* fc, D2D1Resources* d2d1) : S100SpatialObject(d2d1)
{
	type = S100SpatialObjectType::S101Cell;
	m_FileType = S100_FileType::FILE_S_100_VECTOR;
	m_ObejctType = SpatialObjectType::S101Cell;
	//SetAllNumericCode(fc);
}

S101Cell::~S101Cell()
{
	if (isUpdate())
	{
		UpdateRemoveAll();
	}
	else //BASE file
	{
		RemoveAll();
	}
}

int S101Cell::GetCMFX()
{
	return m_dsgir.m_dssi.m_cmfx;
}

int S101Cell::GetCMFY()
{
	return m_dsgir.m_dssi.m_cmfy;
}

int S101Cell::GetCMFZ()
{
	return m_dsgir.m_dssi.m_cmfz;
}

void S101Cell::SetAllNumericCode(FeatureCatalogue* fc)
{
	if (fc)
	{
		// Feature Type
		auto featureTypes = fc->GetFeatureTypes()->GetVecFeatureType();
		for (auto i = featureTypes.begin(); i != featureTypes.end(); i++)
		{
			auto code = (*i)->GetCodeAsWString();
			m_dsgir.GetFeatureTypeCode(code);
		}

		// Information Type
		auto informationTypes = fc->GetInformationTypes()->GetInformationTypePointer();;
		for (auto i = informationTypes.begin(); i != informationTypes.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			m_dsgir.GetInformationTypeCode(code);
		}

		// Simple Attribute
		auto simpleAttributes = fc->GetSimpleAttributes()->GetSimpleAttributePointer();;
		for (auto i = simpleAttributes.begin(); i != simpleAttributes.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			m_dsgir.GetAttributeCode(code);
		}

		// Complex Attribute
		auto complexAttributes = fc->GetComplexAttributes()->GetComplexAttributePointer();
		for (auto i = complexAttributes.begin(); i != complexAttributes.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			m_dsgir.GetAttributeCode(code);
		}

		// Role
		auto roles = fc->GetRoles()->GetRolePointer();
		for (auto i = roles.begin(); i != roles.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			m_dsgir.GetAssociationRoleCode(code);
		}

		// Information Association
		auto informationAssociations = fc->GetInformationAssociations()->GetInformationAssociationPointer();
		for (auto i = informationAssociations.begin(); i != informationAssociations.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			m_dsgir.GetInformationAssociationCode(code);
		}

		// Feature Association
		auto featureAssociations = fc->GetFeatureAssociations()->GetFeatureAssociationPointer();
		for (auto i = featureAssociations.begin(); i != featureAssociations.end(); i++)
		{
			auto code = i->second->GetCodeAsWString();
			m_dsgir.GetFeatureAssociationCode(code);
		}
	}
}

void S101Cell::SetAllCode(FeatureCatalogue* fc)
{
	if (fc)
	{
		for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
		{
			auto feature = (*i);
			feature->SetCode(GetFeatureTypeCodeByID(feature->GetIDAsInteger()));
		}

		for (auto i = vecInformation.begin(); i != vecInformation.end(); i++)
		{
			auto information = (*i);
			information->SetCode(GetInformationTypeCodeByID(information->GetIDAsInteger()));
		}
	}
}

void S101Cell::SetAllCode()
{
	auto fc = GetFC();
	SetAllCode(fc);
}

R_DSGIR* S101Cell::GetDatasetGeneralInformationRecord()
{
	return &m_dsgir;
}

void S101Cell::UpdateRemoveAll(void)
{
	if (nullptr != updateInformation)
	{
		updateInformation->ClearAll();
		delete updateInformation;
	}

	for (auto i = updates.begin(); i != updates.end(); i++)
	{
		auto item = *i;
		delete item;
	}
	updates.clear();

	m_feaMatchingKeys.clear();
	m_infMatchingKeys.clear();
}

void S101Cell::RemoveAll(void)
{
	m_dsgir.init();

	if (nullptr != updateInformation)
	{
		updateInformation->ClearAll();
		delete updateInformation;
	}

	for (auto i = updates.begin(); i != updates.end(); i++)
	{
		auto item = *i;
		delete item;
	}
	updates.clear();

	POSITION pos = NULL;
	__int64 iKey;
	R_InformationRecord* ir;
	pos = m_infMap.GetStartPosition();
	while (pos != NULL)
	{
		m_infMap.GetNextAssoc(pos, iKey, ir);
		delete ir;
	}
	m_infMap.RemoveAll();

	R_PointRecord* pr;
	pos = m_ptMap.GetStartPosition();
	while (pos != NULL)
	{
		m_ptMap.GetNextAssoc(pos, iKey, pr);
		delete pr;
	}
	m_ptMap.RemoveAll();

	R_MultiPointRecord* mr;
	pos = m_mpMap.GetStartPosition();
	while (pos != NULL)
	{
		m_mpMap.GetNextAssoc(pos, iKey, mr);
		delete mr;
	}
	m_mpMap.RemoveAll();

	R_CurveRecord* cr;
	pos = m_curMap.GetStartPosition();
	while (pos != NULL)
	{
		m_curMap.GetNextAssoc(pos, iKey, cr);
		delete cr;
	}
	m_curMap.RemoveAll();

	R_CompositeRecord* ccr;
	pos = m_comMap.GetStartPosition();
	while (pos != NULL)
	{
		m_comMap.GetNextAssoc(pos, iKey, ccr);
		delete ccr;
	}
	m_comMap.RemoveAll();

	R_SurfaceRecord* sr;
	pos = m_surMap.GetStartPosition();
	while (pos != NULL)
	{
		m_surMap.GetNextAssoc(pos, iKey, sr);
		delete sr;
	}
	m_surMap.RemoveAll();

	R_FeatureRecord* fr;
	pos = m_feaMap.GetStartPosition();
	while (pos != NULL)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		delete fr;
	}
	m_feaMap.RemoveAll();

	m_feaMatchingKeys.clear();
	m_infMatchingKeys.clear();
}

void S101Cell::ClearAll(void)
{
	m_infMap.RemoveAll();
	m_ptMap.RemoveAll();
	m_mpMap.RemoveAll();
	m_curMap.RemoveAll();
	m_comMap.RemoveAll();
	m_surMap.RemoveAll();
	m_feaMap.RemoveAll();

	m_feaMatchingKeys.clear();
	m_infMatchingKeys.clear();
}

#pragma warning(disable:4018)
bool S101Cell::Open(CString _filepath) // Dataset start, read .000 
{
	auto extension = LibMFCUtil::GetExtension(_filepath);
	if ((extension.CompareNoCase(L"000") >= 0) &&
		(extension.CompareNoCase(L"999") <= 0))
	{
		return OpenBy000(_filepath);
	}
	else if (extension.CompareNoCase(L"gml") == 0)
	{
		return OpenByGML(_filepath);
	}

	return false;
}

bool S101Cell::OpenMetadata(CString _filepath)
{
	auto extension = LibMFCUtil::GetExtension(_filepath);
	if ((extension.CompareNoCase(L"000") >= 0) &&
		(extension.CompareNoCase(L"999") <= 0))
	{
		return OpenMetadataBy000(_filepath);
	}
	else if (extension.CompareNoCase(L"gml") == 0)
	{
		return OpenMetadataByGML(_filepath);
	}

	return false;
}

bool S101Cell::Read8211(std::wstring path)
{
	SetFilePath(path.c_str());

	USES_CONVERSION;

	RemoveAll();

	CFile file;
	if (!file.Open(path.c_str(), CFile::modeRead))
	{
		return false;
	}

	BYTE* pBuf = nullptr;
	BYTE* sBuf = nullptr;
	BYTE* endOfBuf = nullptr;

	LONGLONG fileLength = file.GetLength();

	pBuf = new BYTE[(unsigned int)fileLength];
	sBuf = pBuf;

	file.Read(pBuf, (unsigned)fileLength);

	endOfBuf = pBuf + fileLength - 1;

	file.Close();

	ReadDDR(pBuf);

	DRDirectoryInfo drDir;

	int tcnt = 0;
	while (pBuf < endOfBuf)
	{
		auto curRecordAddress = pBuf;

		tcnt++;
		DRReader drReader;
		int subFieldCount = 0;
		drReader.ReadReader(pBuf);
		subFieldCount = (drReader.m_fieldAreaLoc - DR_LENGTH - 1) / (4 + drReader.m_fieldLength + drReader.m_fieldPosition);

		if (subFieldCount < 1)
		{
			continue;
		}

		drDir.ReAllocateDirectory(subFieldCount);

		drDir.ReadDir(drReader, pBuf);

		if (*(pBuf++) != 0x1E)
		{
		}

		if (strcmp(drDir.GetDirectory(0)->tag, "DSID") == 0)
		{
			m_dsgir.ReadRecord(&drDir, pBuf);
		}
		else if (strcmp(drDir.GetDirectory(0)->tag, "CSID") == 0)
		{
			m_dscrs.ReadRecord(&drDir, pBuf);
		}

		else if (strcmp(drDir.GetDirectory(0)->tag, "IRID") == 0)
		{
			R_InformationRecord* r = new R_InformationRecord();
			r->ReadRecord(&drDir, pBuf);
			InsertInformationRecord(r->m_irid.m_name.GetName(), r);
		}

		else if (strcmp(drDir.GetDirectory(0)->tag, "PRID") == 0)
		{
			R_PointRecord* r = new R_PointRecord();
			r->ReadRecord(&drDir, pBuf);
			auto names = r->m_prid.m_name.GetName();
			InsertPointRecord(r->m_prid.m_name.GetName(), r);
		}
		else if (strcmp(drDir.GetDirectory(0)->tag, "MRID") == 0)
		{
			R_MultiPointRecord* r = new R_MultiPointRecord();
			r->ReadRecord(&drDir, pBuf);
			InsertMultiPointRecord(r->m_mrid.m_name.GetName(), r);
		}
		else if (strcmp(drDir.GetDirectory(0)->tag, "CRID") == 0)
		{
			R_CurveRecord* r = new R_CurveRecord();
			r->ReadRecord(&drDir, pBuf);
			InsertCurveRecord(r->m_crid.m_name.GetName(), r);
		}
		else if (strcmp(drDir.GetDirectory(0)->tag, "CCID") == 0)
		{
			R_CompositeRecord* r = new R_CompositeRecord();
			r->ReadRecord(&drDir, pBuf);
			InsertCompositeCurveRecord(r->m_ccid.m_name.GetName(), r);
		}
		else if (strcmp(drDir.GetDirectory(0)->tag, "SRID") == 0)
		{
			R_SurfaceRecord* r = new R_SurfaceRecord();
			r->ReadRecord(&drDir, pBuf);
			InsertSurfaceRecord(r->m_srid.m_name.GetName(), r);
		}
		else if (strcmp(drDir.GetDirectory(0)->tag, "FRID") == 0)
		{
			R_FeatureRecord* r = new R_FeatureRecord();
			r->ReadRecord(&drDir, pBuf);
			InsertFeatureRecord(r->m_frid.m_name.GetName(), r);
		}

		pBuf = curRecordAddress + drReader.m_recordLength;
	}

	delete[] sBuf;

	return true;
}

bool S101Cell::isUpdate()
{
	if (!m_dsgir.m_dsid.m_prof.Compare(L"2")) {
		return true;
	}

	return false;
}

bool S101Cell::OpenBy000(CString path)
{
	SetFilePath(path);

	USES_CONVERSION;

	RemoveAll();

	if (Read8211(std::wstring(path))) {
		MakeFullSpatialData();
		CalcMBR();
		Check();

		if (false == ATTRtoAttribute())
		{
			return false;
		}

		SetAllCode();

		return true;
	}

	return false;
}

bool S101Cell::OpenMetadataBy000(CString path)
{
	USES_CONVERSION;

	CFile file;
	if (!file.Open(path, CFile::modeRead))
	{
		return false;
	}

	BYTE* pBuf = nullptr;
	BYTE* sBuf = nullptr;
	BYTE* endOfBuf = nullptr;

	LONGLONG fileLength = file.GetLength();

	pBuf = new BYTE[(unsigned int)fileLength];
	sBuf = pBuf;

	file.Read(pBuf, (unsigned)fileLength);

	endOfBuf = pBuf + fileLength - 1;

	file.Close();

	ReadDDR(pBuf);

	DRDirectoryInfo drDir;

	int tcnt = 0;
	while (pBuf < endOfBuf)
	{
		auto curRecordAddress = pBuf;

		tcnt++;
		DRReader drReader;
		int subFieldCount = 0;
		drReader.ReadReader(pBuf);
		subFieldCount = (drReader.m_fieldAreaLoc - DR_LENGTH - 1) / (4 + drReader.m_fieldLength + drReader.m_fieldPosition);

		if (subFieldCount < 1)
		{
			continue;
		}

		drDir.ReAllocateDirectory(subFieldCount);

		drDir.ReadDir(drReader, pBuf);

		if (*(pBuf++) != 0x1E)
		{
		}

		if (strcmp(drDir.GetDirectory(0)->tag, "DSID") == 0)
		{
			m_dsgir.ReadRecord(&drDir, pBuf);
			break;
		}

		pBuf = curRecordAddress + drReader.m_recordLength;
	}

	delete[] sBuf;

	return true;
}

bool S101Cell::OpenByGML(CString path)
{
	SetFilePath(path);

	USES_CONVERSION;

	RemoveAll();

	S10XGML* gml = new S10XGML(D2);
	gml->SetLayer(GetLayer());
	gml->Open(path);

	SetAllNumericCode(GetFC());

	ConvertFromS101GML(gml);

	MakeFullSpatialData();

	CalcMBR();
	Check();

	if (gml)
	{
		delete gml;
		gml = nullptr;
	}

	return true;
}

bool S101Cell::OpenMetadataByGML(CString path)
{
	return true;
}

BOOL S101Cell::ReadDDR(BYTE*& buf)
{
	int size = atoi(buf, 5);
	buf -= 5;

	DDR ddr;
	ddr.SetBytes(size, buf);

	buf += ddr.GetSize();

	return TRUE;
}

void S101Cell::SortByFeatureType()
{
	POSITION spasPos = NULL;

	R_FeatureRecord* fr;

	POSITION pos = m_feaMap.GetStartPosition();
	__int64 iKey;

	while (pos)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		if (fr->m_spas.size() == 0)
			continue;
	}
}

BOOL S101Cell::MakeFullSpatialData()
{
	POSITION spasPos = NULL;
	R_FeatureRecord* fr = nullptr;

	POSITION pos = m_feaMap.GetStartPosition();
	__int64 iKey = 0;

	while (pos)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		if (fr->m_spas.size() == 0)
		{
			continue;
		}

		auto rcnm = fr->GetSPASRCNM();
		if (rcnm == 110)
		{
			MakePointData(fr);
		}
		else if (rcnm == 115)
		{
			MakeSoundingData(fr);
		}
		else if (rcnm == 120 || rcnm == 125)
		{
			MakeLineData(fr);
		}
		else if (rcnm == 130)
		{
			MakeAreaData(fr);
		}

		GetFullMaskData(fr);
	}

	return TRUE;
}

BOOL S101Cell::MakePointData(R_FeatureRecord* fe)
{
	if (fe->geometry)
	{
		delete fe->geometry;
		fe->geometry = nullptr;
	}

	R_PointRecord* r = nullptr;

	for (auto itorParent = fe->m_spas.begin(); itorParent != fe->m_spas.end(); itorParent++)
	{
		F_SPAS* spasParent = *itorParent;

		for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
		{
			SPAS* spas = *itor;

			__int64 iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;
			if (m_ptMap.Lookup(iKey, r))
			{
				if (r->m_c2it)
				{
					fe->geometry = new SPoint();
					fe->geometry->SetID(r->GetRCID());
					SPoint* geo = (SPoint*)fe->geometry;
					GetFullSpatialData(r, geo);
				}
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::MakeSoundingData(R_FeatureRecord* fe)
{
	R_MultiPointRecord* r;
	__int64 iKey;

	if (fe->geometry)
	{
		delete fe->geometry;
	}

	fe->geometry = new SMultiPoint();
	SMultiPoint* geo = (SMultiPoint*)fe->geometry;

	for (auto itorParent = fe->m_spas.begin(); itorParent != fe->m_spas.end(); itorParent++)
	{
		F_SPAS* spasParent = *itorParent;

		for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
		{
			SPAS* spas = *itor;

			iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;
			if (m_mpMap.Lookup(iKey, r))
			{
				geo->SetID(r->GetRCID());
				GetFullSpatialData(r, geo);
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::MakeLineData(R_FeatureRecord* fe)
{
	if (fe->geometry)
	{
		delete fe->geometry;
		fe->geometry = nullptr;
	}

	R_CurveRecord* cr = nullptr;
	R_CompositeRecord* ccr = nullptr;

	for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
	{
		F_SPAS* spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			SPAS* spas = *j;
			__int64 iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;

			if (spas->m_name.RCNM == 120)
			{
				if (m_curMap.Lookup(iKey, cr))
				{
					auto sc = new SCurve();
					fe->geometry = sc;
					sc->SetID(cr->GetRCID());
					GetFullSpatialData(cr, sc, spas->m_ornt);
				}
			}
			else if (spas->m_name.RCNM == 125)
			{
				if (m_comMap.Lookup(iKey, ccr))
				{
					auto scc = new SCompositeCurve();
					fe->geometry = scc;
					scc->SetID(ccr->GetRCID());
					GetFullSpatialData(ccr, scc, spas->m_ornt);
				}
			}
		}
	}

	if (fe->geometry)
	{
		fe->geometry->CreateD2Geometry(D2->pD2Factory);
	}

	return TRUE;
}

// France
BOOL S101Cell::MakeAreaData(R_FeatureRecord* fe)
{
	if (fe->geometry)
	{
		delete fe->geometry;
		fe->geometry = nullptr;
	}

	R_SurfaceRecord* sr = nullptr;
	__int64 iKey;

	std::vector<POINT> vecPoint;
	std::vector<int> boundaryList;

	fe->geometry = new SSurface();

	SSurface* geo = ((SSurface*)fe->geometry);

	for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
	{
		F_SPAS* spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			SPAS* spas = *j;

			iKey = spas->m_name.GetName();

			if (m_surMap.Lookup(iKey, sr))
			{
				geo->SetID(sr->GetRCID());

				for (auto k = sr->m_rias.begin(); k != sr->m_rias.end(); k++)
				{
					F_RIAS* riasParent = *k;

					for (auto l = riasParent->m_arr.begin(); l != riasParent->m_arr.end(); l++)
					{
						RIAS* rias = *l;

						auto iKey = rias->m_name.GetName();
						if (rias->m_name.RCNM == 120)
						{
							R_CurveRecord* cr = nullptr;
							if (m_curMap.Lookup(iKey, cr))
							{
								SCurve* sCurve = new SCurve();
								sCurve->SetID(cr->GetRCID());
								GetFullSpatialData(cr, sCurve, rias->m_ornt);
								GetFullSpatialData(cr, vecPoint, rias->m_ornt);
								geo->AddCurve(sCurve);
							}
							else
							{
								return FALSE;
							}
						}
						else if (rias->m_name.RCNM == 125)
						{
							R_CompositeRecord* ccr = nullptr;
							if (m_comMap.Lookup(iKey, ccr))
							{
								auto compositeCurve = new SCompositeCurve();
								compositeCurve->SetID(ccr->GetRCID());
								GetFullSpatialData(ccr, compositeCurve, rias->m_ornt);
								GetFullSpatialData(ccr, vecPoint, rias->m_ornt);
								geo->AddCurve(compositeCurve);
							}
							else
							{
								return FALSE;
							}
						}
						else
						{
							return FALSE;
						}


						int sizet = (int)vecPoint.size();

						if (sizet == 0)
						{
							continue;
						}

						boundaryList.push_back(sizet);
					}
				}
			}
		}
	}

	geo->Set(vecPoint, boundaryList);

	if (D2 == nullptr)
	{
		return false;
	}

	geo->SetMBR();
	geo->CreateD2Geometry(D2->pD2Factory);
	geo->CalculateCenterPoint();

	if (sr)
	{
		for (auto i = sr->m_inas.begin(); i != sr->m_inas.end(); i++)
		{
			auto f_inas = *i;
			auto infoKey = f_inas->m_name.GetName();
			R_InformationRecord* info = nullptr;
			if (TRUE == m_infMap.Lookup(infoKey, info))
			{
				geo->AddInformationType(info);
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_PointRecord* r, SPoint* point)
{
	double x = 0; r->m_c2it->m_xcoo;
	double y = 0; r->m_c2it->m_ycoo;

	if (r->m_c2it)
	{
		x = r->m_c2it->m_xcoo;
		y = r->m_c2it->m_ycoo;
	}
	else if (r->m_c3it)
	{
		x = r->m_c3it->m_xcoo;
		y = r->m_c3it->m_ycoo;
	}

	if (m_dsgir.m_dssi.m_cmfy && m_dsgir.m_dssi.m_cmfx)
	{
		point->SetPoint(
			x / (double)m_dsgir.m_dssi.m_cmfx,
			y / (double)m_dsgir.m_dssi.m_cmfy);
	}
	else
	{
		point->SetPoint(
			x / 10000000.0,
			y / 10000000.0);
	}

	projection(point->x, point->y);
	point->SetMBR();

	for (auto i = r->m_inas.begin(); i != r->m_inas.end(); i++)
	{
		auto f_inas = *i;
		auto infoKey = f_inas->m_name.GetName();
		R_InformationRecord* info = nullptr;
		if (TRUE == m_infMap.Lookup(infoKey, info))
		{
			point->AddInformationType(info);
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_MultiPointRecord* r, SMultiPoint* multiPoint)
{
	if (r->m_c3il.size() == 1)
	{
		F_C3IL* c3il = r->m_c3il.front();

		auto size = c3il->m_arr.size();
		multiPoint->SetSize((int)size);

		int index = 0;
		for (auto itor = c3il->m_arr.begin(); itor != c3il->m_arr.end(); itor++)
		{
			C3IL* unitC3IL = *itor;

			double x = unitC3IL->m_xcoo / (double)m_dsgir.m_dssi.m_cmfx;
			double y = unitC3IL->m_ycoo / (double)m_dsgir.m_dssi.m_cmfy;
			double z = (unitC3IL->m_zcoo > 0 ? unitC3IL->m_zcoo + 0.5 : unitC3IL->m_zcoo - 0.5) / (double)m_dsgir.m_dssi.m_cmfz;

			projection(x, y);

			multiPoint->Set(index, x, y, z);
			index++;
		}

		multiPoint->SetMBR();

		for (auto i = r->m_inas.begin(); i != r->m_inas.end(); i++)
		{
			auto f_inas = *i;
			auto infoKey = f_inas->m_name.GetName();
			R_InformationRecord* info = nullptr;
			if (TRUE == m_infMap.Lookup(infoKey, info))
			{
				multiPoint->AddInformationType(info);
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL S101Cell::GetFullSpatialData(R_CurveRecord* r, std::vector<POINT>& geoArr, int ORNT)
{
	if (nullptr != r->m_ptas)
	{
		auto countPTAS = r->m_ptas->m_arr.size();

		if (countPTAS != 1 && countPTAS != 2)
		{
			return FALSE;
		}

		auto beginPointKey = r->m_ptas->m_arr.front()->m_name.GetName();
		auto endPointKey = r->m_ptas->m_arr.back()->m_name.GetName();

		auto beginPointRecord = GetPointRecord(beginPointKey);
		if (nullptr == beginPointRecord || nullptr == beginPointRecord->m_c2it)
		{
			return FALSE;
		}

		auto endPointRecord = GetPointRecord(endPointKey);
		if (nullptr == endPointRecord || nullptr == endPointRecord->m_c2it)
		{
			return FALSE;
		}

		// PTAS
		if (1 == ORNT)
		{
			geoArr.push_back({ beginPointRecord->m_c2it->m_xcoo, beginPointRecord->m_c2it->m_ycoo });
		}
		else if (2 == ORNT)
		{
			geoArr.push_back({ endPointRecord->m_c2it->m_xcoo, endPointRecord->m_c2it->m_ycoo });
		}
		else
		{
			return FALSE;
		}

		// C2IL
		if (r->m_c2il.size() == 1)
		{
			// Forward
			if (1 == ORNT)
			{
				for (auto i = r->m_c2il.front()->m_arr.begin(); i != r->m_c2il.front()->m_arr.end(); i++)
				{
					auto segC2IL = *i;
					geoArr.push_back({ segC2IL->m_xcoo, segC2IL->m_ycoo });
				}
			}
			// Reverse
			else if (2 == ORNT)
			{
				for (auto i = r->m_c2il.front()->m_arr.rbegin(); i != r->m_c2il.front()->m_arr.rend(); i++)
				{
					auto segC2IL = *i;
					geoArr.push_back({ segC2IL->m_xcoo, segC2IL->m_ycoo });
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		// PTAS
		if (1 == ORNT)
		{
			geoArr.push_back({ endPointRecord->m_c2it->m_xcoo, endPointRecord->m_c2it->m_ycoo });
		}
		else if (2 == ORNT)
		{
			geoArr.push_back({ beginPointRecord->m_c2it->m_xcoo, beginPointRecord->m_c2it->m_ycoo });
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL S101Cell::GetFullSpatialData(R_CurveRecord* r, SCurve* curve, int ORNT)
{
	int pointCount = r->GetPointCount();
	if (pointCount == 0)
	{
		return FALSE;
	}

	curve->Init(pointCount);
	curve->SetRCID(r->GetRCID());
	int pointIndex = 0;

	if (nullptr != r->m_ptas)
	{
		auto countPTAS = r->m_ptas->m_arr.size();

		if (countPTAS != 1 && countPTAS != 2)
		{
			return FALSE;
		}

		auto beginPointKey = r->m_ptas->m_arr.front()->m_name.GetName();
		auto endPointKey = r->m_ptas->m_arr.back()->m_name.GetName();

		auto beginPointRecord = GetPointRecord(beginPointKey);
		if (nullptr == beginPointRecord || nullptr == beginPointRecord->m_c2it)
		{
			return FALSE;
		}

		auto endPointRecord = GetPointRecord(endPointKey);
		if (nullptr == endPointRecord || nullptr == endPointRecord->m_c2it)
		{
			return FALSE;
		}

		// PTAS
		if (1 == ORNT)
		{
			curve->Set(pointIndex, beginPointRecord->m_c2it->m_xcoo, beginPointRecord->m_c2it->m_ycoo);
			curve->GetPoint(pointIndex)->SetID(beginPointRecord->GetRCID());
			pointIndex++;
		}
		else if (2 == ORNT)
		{
			curve->Set(pointIndex, endPointRecord->m_c2it->m_xcoo, endPointRecord->m_c2it->m_ycoo);
			curve->GetPoint(pointIndex)->SetID(endPointRecord->GetRCID());
			pointIndex++;
		}
		else
		{
			return FALSE;
		}

		// C2IL
		if (r->m_c2il.size() == 1)
		{
			// Forward
			if (1 == ORNT)
			{
				for (auto i = r->m_c2il.front()->m_arr.begin(); i != r->m_c2il.front()->m_arr.end(); i++)
				{
					auto segC2IL = *i;
					curve->Set(pointIndex++, segC2IL->m_xcoo, segC2IL->m_ycoo);
				}
			}
			// Reverse
			else if (2 == ORNT)
			{
				for (auto i = r->m_c2il.front()->m_arr.rbegin(); i != r->m_c2il.front()->m_arr.rend(); i++)
				{
					auto segC2IL = *i;
					curve->Set(pointIndex++, segC2IL->m_xcoo, segC2IL->m_ycoo);
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		// PTAS
		if (1 == ORNT)
		{
			curve->Set(pointIndex, endPointRecord->m_c2it->m_xcoo, endPointRecord->m_c2it->m_ycoo);
			curve->GetPoint(pointIndex)->SetID(endPointRecord->GetRCID());
			pointIndex++;
		}
		else if (2 == ORNT)
		{
			curve->Set(pointIndex, beginPointRecord->m_c2it->m_xcoo, beginPointRecord->m_c2it->m_ycoo);
			curve->GetPoint(pointIndex)->SetID(beginPointRecord->GetRCID());
			pointIndex++;
		}
		else
		{
			return FALSE;
		}

		curve->SetMultiplicationFactor(m_dsgir.m_dssi.m_cmfx, m_dsgir.m_dssi.m_cmfy);
		curve->Projection();
		curve->SetMBR();

		for (auto i = r->m_inas.begin(); i != r->m_inas.end(); i++)
		{
			auto f_inas = *i;
			auto infoKey = f_inas->m_name.GetName();
			R_InformationRecord* info = nullptr;
			if (TRUE == m_infMap.Lookup(infoKey, info))
			{
				curve->AddInformationType(info);
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL S101Cell::GetFullSpatialData(R_CompositeRecord* r, SCompositeCurve* curve, int ORNT)
{
	//if (r->m_cuco.size() != 1)
	//{
	//	return FALSE;
	//}

	for (auto i = r->m_cuco.begin(); i != r->m_cuco.end(); i++) {
		auto f_cuco = *i;
		for (auto j = f_cuco->m_arr.begin(); j != f_cuco->m_arr.end(); j++) {
			auto cuco = *j;

			auto iKey = cuco->m_name.GetName();

			if (cuco->m_name.RCNM == 120)
			{
				R_CurveRecord* cr = nullptr;

				if (m_curMap.Lookup(iKey, cr))
				{
					SCurve* scurve = new SCurve();
					scurve->SetID(cuco->m_name.RCID);
					int localORNT = cuco->m_ornt;

					if (ORNT == 2)
					{
						if (cuco->m_ornt == 1)
						{
							localORNT = 2;
						}
						else if (cuco->m_ornt == 2)
						{
							localORNT = 1;
						}
					}

					if (GetFullSpatialData(cr, scurve, localORNT))
					{
						curve->AddCurve(scurve);
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					// Failed to find curve record
					return FALSE;
				}
			}
			else if (cuco->m_name.RCNM == 125)
			{
				R_CompositeRecord* ccr = nullptr;

				if (m_comMap.Lookup(iKey, ccr))
				{
					auto scc = new SCompositeCurve();
					scc->SetID(cuco->m_name.RCID);
					GetFullSpatialData(ccr, scc, cuco->m_ornt);
					curve->AddCurve(scc);
				}
				else
				{
					// Failed to find composite curve record
					return FALSE;
				}
			}
			else
			{
				// Invalid rcnm
				return FALSE;
			}
		}
	}

	curve->SetMBR();

	for (auto i = r->m_inas.begin(); i != r->m_inas.end(); i++)
	{
		auto f_inas = *i;
		auto infoKey = f_inas->m_name.GetName();
		R_InformationRecord* info = nullptr;
		if (TRUE == m_infMap.Lookup(infoKey, info))
		{
			curve->AddInformationType(info);
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_CompositeRecord* r, std::vector<POINT>& geoArr, int ORNT)
{
	for (auto i = r->m_cuco.begin(); i != r->m_cuco.end(); i++)
	{
		auto CUCO = *i;
		for (auto j = CUCO->m_arr.begin(); j != CUCO->m_arr.end(); j++)
		{
			auto segCUCO = *j;

			if (segCUCO->m_name.RCNM == 120)
			{
				auto curveRecord = GetCurveRecord(segCUCO->m_name.GetName());
				if (nullptr != curveRecord)
				{
					GetFullSpatialData(curveRecord, geoArr, segCUCO->m_ornt);
				}
			}
			else if (segCUCO->m_name.RCNM == 125)
			{
				auto compositeCurveRecord = GetCompositeCurveRecord(segCUCO->m_name.GetName());
				if (nullptr != compositeCurveRecord)
				{
					GetFullSpatialData(compositeCurveRecord, geoArr, segCUCO->m_ornt);
				}
			}
			else
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullMaskData(R_FeatureRecord* fe)
{
	std::list<SCurve*> listCurveLink;

	if (fe->geometry == nullptr)
	{
		return FALSE;
	}

	if (fe->geometry->GetType() == SGeometryType::CompositeCurve)
	{
		SCompositeCurve* geo = (SCompositeCurve*)fe->geometry;
		geo->GetCurveList(listCurveLink);
	}
	else if (fe->geometry->GetType() == SGeometryType::Surface)
	{
		SSurface* geo = (SSurface*)fe->geometry;
		geo->GetCurveList(listCurveLink);
	}
	else if (fe->geometry->GetType() == SGeometryType::Curve)
	{
		listCurveLink.push_back((SCurve*)fe->geometry);
	}
	else
	{
		return FALSE;
	}

	for (auto i = listCurveLink.begin(); i != listCurveLink.end(); i++)
	{
		auto c = *i;

		for (auto itorParent = fe->m_mask.begin(); itorParent != fe->m_mask.end(); itorParent++)
		{
			F_MASK* maskParent = *itorParent;
			auto mi = maskParent->GetMask(RecordName(120, c->GetRCID()));
			if (mi)
			{
				c->SetMasking(true);
			}
		}
	}

	return TRUE;
}

void S101Cell::Draw(D2D1Resources* D2, Scaler* scaler)
{
	auto rt = D2->pRT;
	rt->FillRectangle(D2D1::RectF(0, 0, 100, 100), D2->pBrush);
}

void S101Cell::Draw(HDC& hDC, Scaler* scaler, double offset)
{
}

void S101Cell::Draw(HDC& hDC, Scaler* scaler, int priority, int instructionType, double offset)
{
}

MBR S101Cell::CalcMBR()
{
	if (nullptr == m_pLayer)
	{
		return MBR();
	}

	auto pMBR = m_pLayer->GetMBRPointer();

	if (nullptr == pMBR)
	{
		return MBR();
	}

	R_FeatureRecord* fr = nullptr;

	__int64 iKey;
	POSITION pos = m_feaMap.GetStartPosition();
	while (pos != NULL)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		if (fr->geometry)
		{
			if (fr->geometry->GetType() == SGeometryType::Surface)
			{
				SSurface* pSr = (SSurface*)fr->geometry;
				pMBR->ReMBR(pSr->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::CompositeCurve)
			{
				auto geo = (SCompositeCurve*)fr->geometry;
				pMBR->ReMBR(geo->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::Point)
			{
				SPoint* geo = (SPoint*)fr->geometry;
				pMBR->ReMBR(geo->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::MultiPoint)
			{
				SMultiPoint* geo = (SMultiPoint*)fr->geometry;
				pMBR->ReMBR(geo->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::Curve)
			{
				SCurve* geo = (SCurve*)fr->geometry;
				pMBR->ReMBR(geo->m_mbr);
			}
		}
	}

	return *pMBR;
}

MBR S101Cell::ReMBR()
{
	MBR result;
	result.InitMBR();

	R_FeatureRecord* fr = nullptr;

	__int64 iKey;
	POSITION pos = m_feaMap.GetStartPosition();
	while (pos != NULL)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		if (fr->geometry)
		{
			if (fr->geometry->GetType() == SGeometryType::Surface)
			{
				SSurface* pSr = (SSurface*)fr->geometry;
				result.ReMBR(pSr->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::CompositeCurve)
			{
				auto geo = (SCompositeCurve*)fr->geometry;
				result.ReMBR(geo->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::Point)
			{
				SPoint* geo = (SPoint*)fr->geometry;
				result.ReMBR(geo->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::MultiPoint)
			{
				SMultiPoint* geo = (SMultiPoint*)fr->geometry;
				result.ReMBR(geo->m_mbr);
			}
			else if (fr->geometry->GetType() == SGeometryType::Curve)
			{
				SCurve* geo = (SCurve*)fr->geometry;
				result.ReMBR(geo->m_mbr);
			}
		}
	}

	if (m_pLayer)
	{
		m_pLayer->SetMBR(result);
	}

	return result;
}

void S101Cell::SetEncodingSpecification(CString value)
{
	m_dsgir.m_dsid.m_ensp = value;
}
CString S101Cell::GetEncodingSpecification()
{
	return m_dsgir.m_dsid.m_ensp;
}
std::string S101Cell::GetEncodingSpecificationToString()
{
	return pugi::as_utf8(GetEncodingSpecification());
}

void S101Cell::SetEncodingSpecificationEdition(CString value)
{
	m_dsgir.m_dsid.m_ened = value;
}

CString S101Cell::GetEncodingSpecificationEdition()
{
	return m_dsgir.m_dsid.m_ened;
}

std::string S101Cell::GetEncodingSpecificationEditionToString()
{
	return pugi::as_utf8(GetEncodingSpecificationEdition());
}

void S101Cell::SetProductIdentifier(CString value)
{
	m_dsgir.m_dsid.m_prsp = value;
}

CString S101Cell::GetProductIdentifier()
{
	return m_dsgir.m_dsid.m_prsp;
}

std::string S101Cell::GetProductIdentifierToString()
{
	return pugi::as_utf8(GetProductIdentifier());
}

void S101Cell::SetProductEdition(CString value)
{
	m_dsgir.m_dsid.m_pred = value;
}

CString S101Cell::GetProductEdition()
{
	return m_dsgir.m_dsid.m_pred;
}

std::string S101Cell::GetProductEditionToString()
{
	return pugi::as_utf8(GetProductEdition());
}

void S101Cell::SetApplictionProfile(CString value)
{
	m_dsgir.m_dsid.m_prof = value;
}

CString S101Cell::GetApplicationProfile()
{
	return m_dsgir.m_dsid.m_prof;
}

std::string S101Cell::GetApplicationProfileToString()
{
	return pugi::as_utf8(GetApplicationProfile());
}

void S101Cell::SetDatasetFileIdentifier(CString value)
{
	m_dsgir.m_dsid.m_dsnm = value;
	SetFileName(value);
}

CString S101Cell::GetDatasetFileIdentifier()
{
	return m_dsgir.m_dsid.m_dsnm;
}

void S101Cell::SetDatasetTitle(CString value)
{
	m_dsgir.m_dsid.m_dstl = value;
}

CString S101Cell::GetDatasetTitle()
{
	return m_dsgir.m_dsid.m_dstl;
}

std::string S101Cell::GetDatasetTitleToString()
{
	return pugi::as_utf8(GetDatasetTitle());
}

void S101Cell::SetDatasetReferenceDate(CString value)
{
	m_dsgir.m_dsid.m_dsrd = value;
}

CString S101Cell::GetDatasetReferenceDate()
{
	return m_dsgir.m_dsid.m_dsrd;
}

std::string S101Cell::GetDatasetReferenceDataToString()
{
	return pugi::as_utf8(GetDatasetReferenceDate());
}

void S101Cell::SetDatasetLanguage(CString value)
{
	m_dsgir.m_dsid.m_dslg = value;
}

CString S101Cell::GetDatasetLanguage()
{
	return m_dsgir.m_dsid.m_dslg;
}

std::string S101Cell::GetDatasetLanguageToString()
{
	return pugi::as_utf8(GetDatasetLanguage());
}

void S101Cell::SetDatasetAbstract(CString value)
{
	m_dsgir.m_dsid.m_dsab = value;
}

CString S101Cell::GetDatasetAbstract()
{
	return m_dsgir.m_dsid.m_dsab;
}

std::string S101Cell::GetDatasetAbstractToString()
{
	return pugi::as_utf8(GetDatasetAbstract());
}

void S101Cell::SetDatasetEdition(CString value)
{
	m_dsgir.m_dsid.m_dsed = value;
}

CString S101Cell::GetDatasetEdition()
{
	return m_dsgir.m_dsid.m_dsed;
}

std::string S101Cell::GetDatasetEditionToString()
{
	return  pugi::as_utf8(GetDatasetEdition());
}

std::string S101Cell::GetDatasetFileIdentifierToString()
{
	return pugi::as_utf8(std::wstring(GetDatasetFileIdentifier()));
}

void S101Cell::InsertRecord(Record* record)
{
	auto RCNM = record->GetRecordName().RCNM;
	auto key = record->GetRecordName().GetName();
	if (100 == RCNM)
	{
		auto featureRecord = (R_FeatureRecord*)record;
		InsertFeatureRecord(key, featureRecord);
	}
	else if (110 == RCNM)
	{
		auto pointRecord = (R_PointRecord*)record;
		InsertPointRecord(key, pointRecord);
	}
	else if (115 == RCNM)
	{
		auto multiPointRecord = (R_MultiPointRecord*)record;
		InsertMultiPointRecord(key, multiPointRecord);
	}
	else if (120 == RCNM)
	{
		auto curveRecord = (R_CurveRecord*)record;
		InsertCurveRecord(key, curveRecord);
	}
	else if (125 == RCNM)
	{
		auto compositeCurveRecord = (R_CompositeRecord*)record;
		InsertCompositeCurveRecord(key, compositeCurveRecord);
	}
	else if (130 == RCNM)
	{
		auto surfaceRecord = (R_SurfaceRecord*)record;
		InsertSurfaceRecord(key, surfaceRecord);
	}
	else if (150 == RCNM)
	{
		auto informationRecord = (R_InformationRecord*)record;
		InsertInformationRecord(key, informationRecord);
	}
}

void S101Cell::InsertInformationRecord(__int64 key, R_InformationRecord* record)
{
	R_InformationRecord* informationRecord = nullptr;
	if (TRUE == m_infMap.Lookup(key, informationRecord))
	{
		if (!informationRecord)
		{
			m_infMap.SetAt(key, record);
			vecInformation.push_back(record);
		}
	}
	else
	{
		m_infMap.SetAt(key, record);
		vecInformation.push_back(record);
	}
}

void S101Cell::RemoveInformationRecord(__int64 key)
{
	R_InformationRecord* informationRecord = nullptr;
	if (TRUE == m_infMap.Lookup(key, informationRecord))
	{
		if (informationRecord)
		{
			m_infMap.RemoveKey(key);
			vecInformation.erase(std::remove(vecInformation.begin(), vecInformation.end(), informationRecord), vecInformation.end());

			delete informationRecord;
		}
	}
}

R_InformationRecord* S101Cell::GetInformationRecord(__int64 key)
{
	if (m_infMatchingKeys.size() > 0)
	{
		auto iter = std::find(m_infMatchingKeys.begin(), m_infMatchingKeys.end(), key);
		if (iter == m_infMatchingKeys.end())
			return nullptr;
	}

	R_InformationRecord* item = nullptr;
	if (TRUE == m_infMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

R_InformationRecord* S101Cell::GetInformationRecord(std::string key)
{
	auto iKey = std::stoll(key);
	RecordName recordName(150, (int)iKey);
	return GetInformationRecord(recordName.GetName());
}

R_InformationRecord* S101Cell::GetInformationRecord(std::wstring wstringKey)
{
	auto rcid = std::stoll(wstringKey);
	return GetInformationRecord(rcid);
}

R_InformationRecord* S101Cell::GetInformationRecordByIndex(int index)
{
	if (index >= 0 && index < vecInformation.size())
	{
		return vecInformation.at(index);
	}

	return nullptr;
}

POSITION S101Cell::GetInfoStartPosition()
{
	return m_infMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_InformationRecord*& value)
{
	if (m_infMatchingKeys.size() > 0)
	{
		while (true)
		{
			m_infMap.GetNextAssoc(index, key, value);
			if (index == nullptr)
			{
				key = -1;
				value = nullptr;
				return;
			}

			auto iter = std::find(m_infMatchingKeys.begin(), m_infMatchingKeys.end(), key);
			if (iter != m_infMatchingKeys.end())
				return;
		}
	}
	else
		m_infMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveAllInfoRecord()
{
	m_infMap.RemoveAll();
}

int S101Cell::GetInfoMapCount()
{
	return (int)m_infMap.GetCount();
}

std::vector<R_InformationRecord*>& S101Cell::GetVecInformation()
{
	return vecInformation;
}

void S101Cell::InsertInformationFilter(__int64 key)
{	
	m_infMatchingKeys.push_back(key);
}

void S101Cell::InsertInformationFilter(std::string key)
{
	auto iKey = std::stoll(key);
	RecordName recordName(RCNM::InformationType, (int)iKey);
	return InsertInformationFilter(recordName.GetName());
}

void S101Cell::InsertInformationFilter(std::wstring wstringKey)
{
	auto key = std::stoll(wstringKey);
	return InsertInformationFilter(key);
}

void S101Cell::RemoveInformationFilter()
{
	m_infMatchingKeys.clear();
}

std::vector<__int64>& S101Cell::GetInformationFilter()
{
	return m_infMatchingKeys;
}

void S101Cell::InsertPointRecord(__int64 key, R_PointRecord* record)
{
	auto pointRecord = GetPointRecord(key);
	if (nullptr == pointRecord)
	{
		m_ptMap.SetAt(key, record);
		vecPoint.push_back(record);
	}
}

void S101Cell::RemovePointRecord(__int64 key, R_PointRecord* record)
{
	m_ptMap.RemoveKey(key);
}

R_PointRecord* S101Cell::GetPointRecord(__int64 key)
{
	R_PointRecord* item = nullptr;
	if (TRUE == m_ptMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

R_PointRecord* S101Cell::GetPointRecordByIndex(int index)
{
	if (index >= 0 && index < vecPoint.size())
	{
		return vecPoint.at(index);
	}

	return nullptr;
}

POSITION S101Cell::GetpointStartPosition()
{
	return m_ptMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_PointRecord*& value)
{
	return m_ptMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveAllPointRecord()
{
	m_ptMap.RemoveAll();
}

std::vector<R_PointRecord*>& S101Cell::GetVecPoint()
{
	return vecPoint;
}

void S101Cell::InsertMultiPointRecord(__int64 key, R_MultiPointRecord* record)
{
	auto multiPointRecord = GetMultiPointRecord(key);
	if (nullptr == multiPointRecord)
	{
		m_mpMap.SetAt(key, record);
		vecMultiPoint.push_back(record);
	}
}

void S101Cell::RemoveMultiPointRecord(__int64 key)
{
	m_mpMap.RemoveKey(key);

	for (auto i = vecMultiPoint.begin(); i != vecMultiPoint.end(); i++)
	{
		if (key == (*i)->GetRecordName().GetName())
		{
			delete (*i);
			vecMultiPoint.erase(i);
			return;
		}
	}
}

R_MultiPointRecord* S101Cell::GetMultiPointRecord(__int64 key)
{
	R_MultiPointRecord* item = nullptr;
	if (TRUE == m_mpMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

R_MultiPointRecord* S101Cell::GetMultiPointRecordByIndex(int index)
{
	if (index >= 0 && index < vecMultiPoint.size())
	{
		return vecMultiPoint.at(index);
	}

	return nullptr;
}

POSITION S101Cell::GetMultStartPosition()
{
	return m_mpMap.GetStartPosition();
}


void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_MultiPointRecord*& value)
{
	m_mpMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveAllMultRecord()
{
	m_mpMap.RemoveAll();
}

std::vector<R_MultiPointRecord*>& S101Cell::GetVecMultiPoint()
{
	return vecMultiPoint;
}

bool S101Cell::InsertCurveRecord(__int64 key, R_CurveRecord* record)
{
	auto curveRecord = GetCurveRecord(key);
	if (curveRecord == nullptr) {
		m_curMap.SetAt(key, record);
		vecCurve.push_back(record);
		return true;
	}

	return false;
}

void S101Cell::RemoveCurveRecord(__int64 key, R_CurveRecord* record)
{
	m_curMap.RemoveKey(key);
}

R_CurveRecord* S101Cell::GetCurveRecord(__int64 key)
{
	R_CurveRecord* item = nullptr;
	if (TRUE == m_curMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

R_CurveRecord* S101Cell::GetCurveRecordByIndex(int index)
{
	if (index >= 0 && index < vecCurve.size())
	{
		return vecCurve.at(index);
	}

	return nullptr;
}

POSITION S101Cell::GetCurStartPosition()
{
	return m_curMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_CurveRecord*& value)
{
	m_curMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveAllCurRecord()
{
	m_curMap.RemoveAll();
}

std::vector<R_CurveRecord*>& S101Cell::GetVecCurve()
{
	return vecCurve;
}

void S101Cell::InsertCompositeCurveRecord(__int64 key, R_CompositeRecord* record)
{
	auto compositeCurveRecord = GetCompositeCurveRecord(key);
	if (nullptr == compositeCurveRecord)
	{
		m_comMap.SetAt(key, record);
		vecComposite.push_back(record);
	}
}

void S101Cell::RemoveCompositeCurveRecord(__int64 key, R_CompositeRecord* record)
{
	m_comMap.RemoveKey(key);
}

R_CompositeRecord* S101Cell::GetCompositeCurveRecord(__int64 key)
{
	R_CompositeRecord* item = nullptr;
	if (TRUE == m_comMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

R_CompositeRecord* S101Cell::GetCompositeCurveRecordByIndex(int index)
{
	if (index >= 0 && index < vecComposite.size())
	{
		return vecComposite.at(index);
	}

	return nullptr;
}

POSITION S101Cell::GetComStartPosition()
{
	return m_comMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_CompositeRecord*& value)
{
	m_comMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveAllComRecord()
{
	m_comMap.RemoveAll();
}

std::vector<R_CompositeRecord*>& S101Cell::GetVecComposite()
{
	return vecComposite;
}

void S101Cell::InsertSurfaceRecord(__int64 key, R_SurfaceRecord* record)
{
	auto surfaceRecord = GetSurfaceRecord(key);
	if (nullptr == surfaceRecord)
	{
		m_surMap.SetAt(key, record);
		vecSurface.push_back(record);
	}
}

R_SurfaceRecord* S101Cell::GetSurfaceRecordByIndex(int index)
{
	if (index >= 0 && index < vecSurface.size())
	{
		return vecSurface.at(index);
	}

	return nullptr;
}

void S101Cell::RemoveSurfaceRecord(__int64 key, R_SurfaceRecord* record)
{
	m_surMap.RemoveKey(key);
}

R_SurfaceRecord* S101Cell::GetSurfaceRecord(__int64 key)
{
	R_SurfaceRecord* item = nullptr;
	if (TRUE == m_surMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

POSITION S101Cell::GetSurStartPosition()
{
	return m_surMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_SurfaceRecord*& value)
{
	m_surMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveAllSurRecord()
{
	m_surMap.RemoveAll();
}

std::vector<R_SurfaceRecord*>& S101Cell::GetVecSurface()
{
	return vecSurface;
}

void S101Cell::InsertFeatureRecord(__int64 key, R_FeatureRecord* record)
{
	auto featureRecord = GetFeatureRecord(key);
	if (nullptr == featureRecord)
	{
		m_feaMap.SetAt(key, record);
		vecFeature.push_back(record);
	}
}

void S101Cell::RemoveFeatureRecord(__int64 key)
{
	auto featureRecord = GetFeatureRecord(key);
	if (featureRecord)
	{
		m_feaMap.RemoveKey(key);
		vecFeature.erase(std::remove(vecFeature.begin(), vecFeature.end(), featureRecord), vecFeature.end());
		delete featureRecord;
	}
}

R_FeatureRecord* S101Cell::GetFeatureRecord(__int64 key)
{
	R_FeatureRecord* item = nullptr;
	if (TRUE == m_feaMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

R_FeatureRecord* S101Cell::GetFeatureRecord(std::string key)
{
	auto iKey = std::stoll(key);
	RecordName recordName(100, (int)iKey);
	return GetFeatureRecord(recordName.GetName());
}

R_FeatureRecord* S101Cell::GetFeatureRecord(std::wstring wstringKey)
{
	auto key = std::stoll(wstringKey);
	return GetFeatureRecord(key);
}

R_FeatureRecord* S101Cell::GetFeatureRecordByIndex(int index)
{
	if (index >= 0 && index < vecFeature.size())
	{
		return vecFeature.at(index);
	}

	return nullptr;
}

POSITION S101Cell::GetFeatureStartPosition()
{
	return m_feaMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_FeatureRecord*& value)
{
	if (m_feaMatchingKeys.size() > 0)
	{
		while (true)
		{
			m_feaMap.GetNextAssoc(index, key, value);
			if (index == nullptr)
			{
				key = -1;
				value = nullptr;
				return;
			}

			auto iter = std::find(m_feaMatchingKeys.begin(), m_feaMatchingKeys.end(), key);
			if (iter != m_feaMatchingKeys.end())
				return;
		}
	}
	else
		m_feaMap.GetNextAssoc(index, key, value);
}

void S101Cell::RemoveFeatureMapKey(long long key)
{
	m_feaMap.RemoveKey(key);
}

void S101Cell::RemoveAllFeatureRecord()
{
	m_feaMap.RemoveAll();
}

std::vector<R_FeatureRecord*>& S101Cell::GetVecFeature()
{
	return vecFeature;
}

void S101Cell::InsertFeatureFilter(__int64 key)
{
	m_feaMatchingKeys.push_back(key);
}

void S101Cell::InsertFeatureFilter(std::string key)
{
	auto iKey = std::stoll(key);
	RecordName recordName(RCNM::FeatureType, (int)iKey);
	return InsertFeatureFilter(recordName.GetName());
}

void S101Cell::InsertFeatureFilter(std::wstring wstringKey)
{
	auto key = std::stoll(wstringKey);
	return InsertFeatureFilter(key);
}

void S101Cell::RemoveFeatureFilter()
{
	m_feaMatchingKeys.clear();
}

std::vector<__int64>& S101Cell::GetFeatureFilter()
{
	return m_feaMatchingKeys;
}

int S101Cell::GetCount_InformationRecord()
{
	return (int)m_infMap.GetCount();
}

int S101Cell::GetCount_PointRecord()
{
	return (int)m_ptMap.GetCount();
}

int S101Cell::GetCount_MultiPointRecord()
{
	return (int)m_mpMap.GetCount();
}

int S101Cell::GetCount_CurveRecord()
{
	return (int)m_curMap.GetCount();
}

int S101Cell::GetCount_CompositeCurveRecord()
{
	return (int)m_comMap.GetCount();
}

int S101Cell::GetCount_SurfaceRecord()
{
	return (int)m_surMap.GetCount();
}

int S101Cell::GetCount_FeatureTypeRecord()
{
	return (int)m_feaMap.GetCount();
}

int S101Cell::GetMetaCount_InformationRecord()
{
	return m_dsgir.m_dssi.m_noir;
}

int S101Cell::GetMetaCount_PointRecord()
{
	return m_dsgir.m_dssi.m_nopn;
}

int S101Cell::GetMetaCount_MultiPointRecord()
{
	return m_dsgir.m_dssi.m_nomn;
}

int S101Cell::GetMetaCount_CurveRecord()
{
	return m_dsgir.m_dssi.m_nocn;
}

int S101Cell::GetMetaCount_CompositeCurveRecord()
{
	return m_dsgir.m_dssi.m_noxn;
}

int S101Cell::GetMetaCount_SurfaceRecord()
{
	return m_dsgir.m_dssi.m_nosn;
}

int S101Cell::GetMetaCount_FeatureTypeRecord()
{
	return m_dsgir.m_dssi.m_nofr;
}

bool S101Cell::Check()
{
	if (GetCount_InformationRecord() != GetMetaCount_InformationRecord())
	{
		return false;
	}

	if (GetCount_PointRecord() != GetMetaCount_PointRecord())
	{
		return false;
	}

	if (GetCount_MultiPointRecord() != GetMetaCount_MultiPointRecord())
	{
		return false;
	}

	if (GetCount_CurveRecord() != GetMetaCount_CurveRecord())
	{
		return false;
	}

	if (GetCount_CompositeCurveRecord() != GetMetaCount_CompositeCurveRecord())
	{
		return false;
	}

	if (GetCount_SurfaceRecord() != GetMetaCount_SurfaceRecord())
	{
		return false;
	}

	if (GetCount_FeatureTypeRecord() != GetMetaCount_FeatureTypeRecord())
	{
		return false;
	}

	return true;
}

bool S101Cell::Update(S101Cell* cell)
{
	updates.push_back(cell);
	// Update base's dsgir.
	// Update's nc update

	//DSID update
	m_dsgir.m_dsid.m_ensp = cell->m_dsgir.m_dsid.m_ensp;
	m_dsgir.m_dsid.m_ened = cell->m_dsgir.m_dsid.m_ened;
	m_dsgir.m_dsid.m_prsp = cell->m_dsgir.m_dsid.m_prsp;
	m_dsgir.m_dsid.m_pred = cell->m_dsgir.m_dsid.m_pred;
	m_dsgir.m_dsid.m_prof = cell->m_dsgir.m_dsid.m_prof;
	m_dsgir.m_dsid.m_dsnm = cell->m_dsgir.m_dsid.m_dsnm;
	m_dsgir.m_dsid.m_dstl = cell->m_dsgir.m_dsid.m_dstl;
	m_dsgir.m_dsid.m_dsrd = cell->m_dsgir.m_dsid.m_dsrd;
	m_dsgir.m_dsid.m_dslg = cell->m_dsgir.m_dsid.m_dslg;
	m_dsgir.m_dsid.m_dsab = cell->m_dsgir.m_dsid.m_dsab;
	m_dsgir.m_dsid.m_dsed = cell->m_dsgir.m_dsid.m_dsed;

	UpdateDsgirRecord(cell);//fix the table.

	UpdateInfMapRecord(cell);
	UpdatePtMapRecord(cell);
	UpdateMpMapRecord(cell);
	UpdateCurMapRecord(cell);
	UpdateComMapRecord(cell);
	UpdateSurMapRecord(cell);
	UpdateFeaMapRecord(cell);

	return true;
}

//Erase the details of the table and modify the new memory code of the update file.
bool S101Cell::UpdateDsgirRecord(S101Cell* cell)
{
	//ATCS
	F_CodeWithNumericCode* atcs = cell->m_dsgir.m_atcs;
	if (atcs != nullptr)
	{
		for (auto code : atcs->m_arrFindForCode) //there is no value in the base, you need to add the value in the update.
		{
			std::wstring name = code.first;
			CodeWithNumericCode* nc = code.second;

			if (m_dsgir.m_atcs->GetCode(name.c_str()) == 0) //if has not value
			{
				m_dsgir.m_atcs->InsertCodeNumericCode(nc); //Add a value to the base.
			}
		}
	}

	//ITCS
	F_CodeWithNumericCode* itcs = cell->m_dsgir.m_itcs;
	if (itcs != nullptr)
	{
		for (auto code : itcs->m_arrFindForCode)
		{
			std::wstring name = code.first;
			CodeWithNumericCode* nc = code.second;

			if (m_dsgir.m_itcs->GetCode(name.c_str()) == 0)
			{
				m_dsgir.m_itcs->InsertCodeNumericCode(nc);
			}
		}
	}

	//FTCS
	F_CodeWithNumericCode* ftcs = cell->m_dsgir.m_ftcs;
	if (ftcs != nullptr)
	{
		for (auto code : ftcs->m_arrFindForCode)
		{
			std::wstring name = code.first;
			CodeWithNumericCode* nc = code.second;

			if (m_dsgir.m_ftcs->GetCode(name.c_str()) == 0)
			{
				m_dsgir.m_ftcs->InsertCodeNumericCode(nc);
			}
		}

	}

	//IACS
	F_CodeWithNumericCode* iacs = cell->m_dsgir.m_iacs;
	if (iacs != nullptr)
	{
		for (auto code : iacs->m_arrFindForCode)
		{
			std::wstring name = code.first;
			CodeWithNumericCode* nc = code.second;

			if (m_dsgir.m_iacs->GetCode(name.c_str()) == 0)
			{
				m_dsgir.m_iacs->InsertCodeNumericCode(nc);
			}
		}
	}

	//FACS
	F_CodeWithNumericCode* facs = cell->m_dsgir.m_facs;
	if (facs != nullptr)
	{
		for (auto code : facs->m_arrFindForCode)
		{
			std::wstring name = code.first;
			CodeWithNumericCode* nc = code.second;

			if (m_dsgir.m_facs->GetCode(name.c_str()) == 0)
			{
				m_dsgir.m_facs->InsertCodeNumericCode(nc);
			}
		}
	}

	//ARCS
	F_CodeWithNumericCode* arcs = cell->m_dsgir.m_arcs;
	if (arcs != nullptr)
	{
		for (auto code : arcs->m_arrFindForCode)
		{
			std::wstring name = code.first;
			CodeWithNumericCode* nc = code.second;

			if (m_dsgir.m_arcs->GetCode(name.c_str()) == 0)
			{
				m_dsgir.m_arcs->InsertCodeNumericCode(nc);
			}

		}
	}

	//chage Information Record ATTR 
	__int64 infokey;
	POSITION pos = cell->m_infMap.GetStartPosition(); //change Update Attri to NC
	while (pos != nullptr)
	{
		R_InformationRecord* ir = new R_InformationRecord(); //UPdate value
		cell->m_infMap.GetNextAssoc(pos, infokey, ir);

		//change itcs
		auto updateCode = itcs->GetFeatureCode(ir->m_irid.NITC());
		int NewCode = m_dsgir.m_itcs->GetCode(updateCode);
		ir->m_irid.NITC(NewCode);

		UpdateAttrField(ir->m_attr, atcs);      //attr
		UpdateInasField(ir->m_inas, iacs);		//INAS

	}

	//point map
	__int64 posKey;
	pos = cell->m_ptMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_PointRecord* pt = new R_PointRecord();
		cell->m_ptMap.GetNextAssoc(pos, posKey, pt);
		UpdateInasField(pt->m_inas, iacs);

	}

	//multi point
	__int64 muitKey;
	pos = cell->m_mpMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_MultiPointRecord* mt = new R_MultiPointRecord();
		cell->m_mpMap.GetNextAssoc(pos, muitKey, mt);
		UpdateInasField(mt->m_inas, iacs);
	}

	__int64 curKey;
	pos = cell->m_curMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_CurveRecord* cur = new R_CurveRecord();
		cell->m_curMap.GetNextAssoc(pos, curKey, cur);
		UpdateInasField(cur->m_inas, iacs);
		//PATS
	}

	__int64 conpoKey;
	pos = cell->m_comMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_CompositeRecord* com = new R_CompositeRecord();
		cell->m_comMap.GetNextAssoc(pos, conpoKey, com);
		UpdateInasField(com->m_inas, iacs);
	}

	__int64 surKey;
	pos = cell->m_surMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_SurfaceRecord* sur = new R_SurfaceRecord();
		cell->m_surMap.GetNextAssoc(pos, surKey, sur);
		UpdateInasField(sur->m_inas, iacs);
	}

	//change FeatureType Record ATTR
	__int64 feakey;
	pos = cell->m_feaMap.GetStartPosition(); //change Update Attri to NC
	while (pos != nullptr)
	{
		R_FeatureRecord* fe = new R_FeatureRecord(); //UPdate value
		cell->m_feaMap.GetNextAssoc(pos, feakey, fe);

		//FTCS
		auto updateCode = ftcs->GetFeatureCode(fe->m_frid.m_nftc);
		int NewCode = m_dsgir.m_ftcs->GetCode(updateCode);
		fe->m_frid.m_nftc = NewCode;

		//ATTR 
		UpdateAttrField(fe->m_attr, atcs);

		//INAS
		UpdateInasField(fe->m_inas, iacs);

		//FASC
		for (auto f_fasc : fe->m_fasc)
		{
			for (auto fasc : f_fasc->m_arr)
			{
				auto updateCode = facs->GetFeatureCode(fasc->m_natc);
				int NewCode = m_dsgir.m_facs->GetCode(updateCode);

				fasc->m_natc = NewCode;
			}
		}
	}
	return true;
}

// collect the values of update and the NC values from update.
bool S101Cell::UpdateAttrField(std::list<F_ATTR*> Update, F_CodeWithNumericCode* atcs)
{
	for (auto f_attr : Update)
	{
		for (auto attr : f_attr->m_arr)
		{
			CString updateCode = atcs->GetFeatureCode(attr->m_natc); //bring the NC value for Update.
			int NewCode = m_dsgir.m_atcs->GetCode(updateCode); //bring the NC price from the base.

			//all data change
			attr->m_natc = NewCode;
		}
	}
	return true;
}

bool S101Cell::UpdateInasField(std::list<F_INAS*> Update, F_CodeWithNumericCode* iacs)
{
	for (auto f_inas : Update)
	{
		for (auto i = f_inas->m_arr.begin(); i != f_inas->m_arr.end(); i++)
		{
			auto arr = (*i);
			CString updateCode = iacs->GetFeatureCode(arr->m_natc);
			int NewCode = m_dsgir.m_iacs->GetCode(updateCode);

			arr->m_natc = NewCode;
		}
	}
	return true;
}

bool S101Cell::UpdateInfMapRecord(S101Cell* cell)
{
	__int64 key;
	POSITION pos = cell->m_infMap.GetStartPosition();

	while (pos != NULL)
	{
		R_InformationRecord* ir = new R_InformationRecord(); //UPdate  value
		cell->m_infMap.GetNextAssoc(pos, key, ir);

		auto UpdateName = ir->m_irid.m_name.GetName();
		int idvalue = ir->m_irid.m_ruin;

		if (idvalue == 1) //insert
		{
			InsertInformationRecord(UpdateName, ir);
		}
		else //delete , modify
		{
			R_InformationRecord* values = new R_InformationRecord();
			auto isValues = m_infMap.Lookup(UpdateName, values);
			if (isValues == false)
			{
				continue;
			}

			if (idvalue == 2) //Delete
			{
				RemoveInformationRecord(UpdateName);
			}
			else if (idvalue == 3) //Modify
			{
				//IRID
				values->m_irid.m_name.RCNM = ir->m_irid.m_name.RCNM;
				values->m_irid.m_name.RCID = ir->m_irid.m_name.RCID;
				values->m_irid.NITC(ir->m_irid.NITC()); //I'm going to change the numericcode.
				values->m_irid.m_rver = ir->m_irid.m_rver;

				//ATTR
				UpdateAttrRecord(ir->m_attr, values->m_attr);

				//INAS
				UpdateINASRecord(ir->m_inas, values->m_inas);
			}
		}
	}
	return true;
}

bool S101Cell::UpdateAttrRecord(std::list<F_ATTR*> Update, std::list<F_ATTR*> Base)
{
	int i = 0;
	for (auto f_attr : Update)
	{
		auto it = Base.begin();
		std::advance(it, i);

		if (it == Base.end())
		{
			continue;
		}

		for (auto attr : f_attr->m_arr)
		{
			int mission = attr->m_atin;
			if (mission == 1) //insert
			{
				(*it)->m_arr.push_back(attr);
			}

			else if (mission == 2)//delet
			{
				auto Baseattr = (*it)->m_arr;
				auto itor = std::find(Baseattr.begin(), Baseattr.end(), attr);
				if (itor != Baseattr.end())
				{
					Baseattr.erase(itor);
				}

			}

			else if (mission == 3)//modify
			{
				std::vector<ATTR*> Baseattr = (*it)->m_arr;
				for (auto at : Baseattr)
				{
					if (attr->m_natc == at->m_natc)
					{
						at->m_natc = attr->m_natc;
						at->m_atix = attr->m_atix;
						at->m_paix = attr->m_paix;
						at->m_atvl = attr->m_atvl;
						break;
					}
				}
			}
		}
		i++;
	}
	return true;
}

bool S101Cell::UpdateINASRecord(std::list<F_INAS*> Update, std::list<F_INAS*> Base)
{
	int index = 0;
	for (auto f_inas : Update)
	{
		auto update = f_inas->m_name.RCNM;
		int mission = f_inas->m_iuin;

		if (1 == mission) //add
		{
			Base.push_back(f_inas);
		}

		else if (2 == mission) //delete
		{
			Base.remove(f_inas);
		}

		else if (3 == mission) //change 
		{
			auto it = Base.begin();
			std::advance(it, index);
			if (it == Base.end())
			{
				continue;
			}
			auto BaseAttr = (*it);

			for (auto i = f_inas->m_arr.begin(); i != f_inas->m_arr.end(); i++)
			{
				auto arr = *i;
				auto UpdateCode = arr->m_atin;

				if (1 == UpdateCode)
				{
					BaseAttr->m_arr.push_back(arr);
				}
				else if (2 == UpdateCode) //delte
				{
					for (auto j = BaseAttr->m_arr.begin(); j != BaseAttr->m_arr.end(); j++)
					{
						auto basear = *j;
						if (arr->m_natc == basear->m_natc)
						{
							BaseAttr->m_arr.erase(j);
						}
					}
				}
				else if (3 == UpdateCode)//modify
				{
					for (auto j = BaseAttr->m_arr.begin(); j != BaseAttr->m_arr.end(); j++)
					{
						auto basear = *j;
						if (arr->m_natc == basear->m_natc)
						{
							basear = arr;
						}
					}
				}
			}
		}
		index++;
	}
	return false;
}

bool S101Cell::UpdatePtMapRecord(S101Cell* cell) //point
{
	//change point Record value
	__int64 key;
	POSITION pos = cell->m_ptMap.GetStartPosition();

	while (pos != NULL)
	{
		R_PointRecord* r = new R_PointRecord(); //update value
		cell->m_ptMap.GetNextAssoc(pos, key, r);

		//PRID
		auto newName = r->m_prid.m_name.GetName();
		auto mission = r->m_prid.m_ruin;

		if (mission == 1)
		{
			InsertPointRecord(newName, r);
		}
		else
		{
			R_PointRecord* values = new R_PointRecord();
			auto isValues = m_ptMap.Lookup(newName, values);

			if (isValues == false)
			{
				continue;
			}

			if (mission == 2)//Delete
			{
				RemovePointRecord(newName, values);
			}

			else if (mission == 3) //Replace the value. Change the values like INAS and C2IT located below.
			{
				//IRID
				values->m_prid.m_name.RCNM = r->m_prid.m_name.RCNM; //RCNM
				values->m_prid.m_name.RCID = r->m_prid.m_name.RCID; //RCID
				values->m_prid.m_rver = r->m_prid.m_rver; //RVER

				//INAS
				UpdateINASRecord(r->m_inas, values->m_inas);

				//ATTR
				values->SetC2IT(r->m_c2it);
				values->SetC3IT(r->m_c3it);
			}

		}
	}
	return true;
}

bool S101Cell::UpdateMpMapRecord(S101Cell* cell) //multi point
{
	__int64 key;
	POSITION pos = cell->m_mpMap.GetStartPosition();

	while (pos != NULL)
	{
		R_MultiPointRecord* mu = new R_MultiPointRecord();
		cell->m_mpMap.GetNextAssoc(pos, key, mu);

		auto mission = mu->m_mrid.m_ruin;
		auto UpdateName = mu->m_mrid.m_name.GetName();
		if (mission == 1) //insert 
		{
			InsertMultiPointRecord(UpdateName, mu);
		}
		else
		{

			//find Common value
			R_MultiPointRecord* values = new R_MultiPointRecord();
			auto isValus = m_mpMap.Lookup(UpdateName, values);
			if (isValus == false) // no value
			{
				continue;
			}

			if (mission == 2) //Delete
			{
				RemoveMultiPointRecord(UpdateName);
			}
			else if (mission == 3)  //change
			{
				//INAS
				UpdateINASRecord(mu->m_inas, values->m_inas);

				//C2IL
				UpdateC2ILRecord(mu->m_c2il, values->m_c2il, mu->m_cocc);

				UpdateC3ILRecord(mu->m_c3il, values->m_c3il);
				//C3IL
			}
		}
	}
	return true;
}

bool S101Cell::UpdateC2ILRecord(std::list<F_C2IL*> updates, std::list<F_C2IL*> bases, F_COCC* updatemission)
{
	int mission = updatemission->m_coui;
	int index = updatemission->m_coix;
	int tuple = updatemission->m_ncor;

	if (1 == mission)//insert
	{
		for (F_C2IL* update : updates)
		{
			bases.push_back(update);
		}

	}
	else if (2 == mission)
	{
		auto iter = bases.begin();
		std::advance(iter, index);

		bases.erase(iter);
	}
	else if (3 == mission)
	{
		auto itor = bases.begin();
		auto update = updates.begin();

		for (int i = 1; i <= tuple; i++)
		{
			std::advance(itor, index + i);
			std::advance(update, i - 1);
			(*itor) = *update;
		}
	}
	return false;
}

bool S101Cell::UpdateC3ILRecord(std::list<F_C3IL*> Updates, std::list<F_C3IL*> bases)
{
	for (F_C3IL* update : Updates)
	{
		auto it = std::find(bases.begin(), bases.end(), update);
		if (it != bases.end()) //if find value
		{
			*it = update; //change value
		}
	}
	return false;
}

bool S101Cell::UpdateCurMapRecord(S101Cell* cell) //curve Record.
{
	__int64 key;
	POSITION pos = cell->m_curMap.GetStartPosition();

	while (pos != NULL)
	{

		R_CurveRecord* cur = new R_CurveRecord();
		cell->m_curMap.GetNextAssoc(pos, key, cur);

		auto UpdateName = cur->m_crid.m_name.GetName();
		auto mission = cur->m_crid.m_ruin;

		if (mission == 1)
		{
			InsertCurveRecord(UpdateName, cur);
		}
		else
		{
			R_CurveRecord* value = new R_CurveRecord();
			auto isValue = m_curMap.Lookup(UpdateName, value);

			if (isValue == false)
			{
				continue;
			}

			if (mission == 2) //Delete
			{
				RemoveCurveRecord(UpdateName, value);
			}
			else if (mission == 3) //update 
			{
				UpdateINASRecord(cur->m_inas, value->m_inas); //INAS

				//PTAS
				auto BasePTAS = value->m_ptas->m_arr;
				int i = 0;
				for (PTAS* ptas : cur->m_ptas->m_arr)
				{
					BasePTAS.at(i)->m_name.RCNM = ptas->m_name.RCNM;
					BasePTAS.at(i)->m_name.RCID = ptas->m_name.RCID;
					BasePTAS.at(i)->m_topi = ptas->m_topi;
					i++;
				}

				//SECC
				value->m_secc->m_seix = cur->m_secc->m_seix;
				value->m_secc->m_nseg = cur->m_secc->m_nseg;

				//SEGH
				if (cur->m_segh)
				{
					value->m_segh = cur->m_segh;
				}

				//C2IL
				UpdateC2ILRecord(cur->m_c2il, value->m_c2il, cur->m_cocc);
			}
		}
	}
	return true;
}

bool S101Cell::UpdateComMapRecord(S101Cell* cell)// Composite Curve
{
	__int64 key;
	POSITION pos = cell->m_comMap.GetStartPosition();

	while (pos != NULL)
	{
		R_CompositeRecord* com = new R_CompositeRecord();
		cell->m_comMap.GetNextAssoc(pos, key, com);
		auto UpdateName = com->m_ccid.m_name.GetName();
		auto mission = com->m_ccid.m_ruin;

		if (mission == 1)
		{
			InsertCompositeCurveRecord(UpdateName, com);
		}
		else
		{
			R_CompositeRecord* values = new R_CompositeRecord();
			auto isValue = m_comMap.Lookup(UpdateName, values);

			if (isValue == false)
			{
				continue;
			}

			if (mission == 2)
			{
				RemoveCompositeCurveRecord(UpdateName, values);
			}

			else if (mission == 3)
			{
				UpdateINASRecord(com->m_inas, values->m_inas); //INAS

				if (com->m_ccoc != nullptr)
				{
					int ccmission = com->m_ccoc->m_ccui;

					if (1 == ccmission) // add cuco value
					{
						//add value back
						for (auto cuco : com->m_cuco)
						{
							values->m_cuco.push_back(cuco);
						}
					}

					else if (2 == ccmission) //delete cuco value
					{
						int index = com->m_ccoc->m_ccix;
						int count = com->m_ccoc->m_ncco;

						auto cuco = values->m_cuco;

						int indexCount = 0;
						int RemoveCount = 0;

						for (auto iter = cuco.begin(); iter != cuco.end(); iter++)
						{
							if (indexCount <= index)
							{
								cuco.erase(iter);
								RemoveCount++;

								if (RemoveCount == count)
								{
									break;
								}
							}
							indexCount++;
						}
					}

					else //Modify cuco's value.
					{
						int index = com->m_ccoc->m_ccix;
						int count = com->m_ccoc->m_ncco;

						auto cuco = values->m_cuco;

						auto itor = cuco.begin();

						auto comitor = com->m_cuco.begin();

						for (int i = 1; i <= count; i++)
						{
							std::advance(itor, index + i);
							std::advance(comitor, i - 1);
							(*itor) = *comitor;
						}
					}
				}
			}
		}
	}
	return true;
}

bool S101Cell::UpdateSurMapRecord(S101Cell* cell)
{
	__int64 key;
	POSITION pos = cell->m_surMap.GetStartPosition();
	while (pos != NULL)
	{
		R_SurfaceRecord* sur = new R_SurfaceRecord();
		cell->m_surMap.GetNextAssoc(pos, key, sur);

		auto UpdateName = sur->m_srid.m_name.GetName();
		auto mission = sur->m_srid.m_ruin;

		if (mission == 1)
		{
			InsertSurfaceRecord(UpdateName, sur);
		}
		else
		{
			R_SurfaceRecord* values = new R_SurfaceRecord();
			auto isValues = m_surMap.Lookup(UpdateName, values);

			if (isValues == false)
			{
				continue;
			}

			if (mission == 2)//delete
			{
				RemoveSurfaceRecord(UpdateName, values);
			}
			else if (mission == 3) //change
			{
				UpdateINASRecord(sur->m_inas, values->m_inas); //INAS
				// RIAS

				int i = 0;
				for (F_RIAS* f_rias : sur->m_rias)
				{
					int j = 0;
					for (RIAS* rias : f_rias->m_arr)
					{
						int mission = rias->m_raui;

						auto it = values->m_rias.begin();
						std::advance(it, i);
						auto Basearr = (*it)->m_arr;
						if (mission == 1) //insert
						{
							Basearr.push_back(rias);
						}

						else if (mission == 2) //delete
						{
							auto isArr = std::find(Basearr.begin(), Basearr.end(), rias);
							if (isArr != Basearr.end())
							{
								Basearr.erase(isArr);
							}
						}
					}
					i++;
				}
			}
		}
	}
	return true;
}

bool S101Cell::UpdateFeaMapRecord(S101Cell* cell)
{

	__int64 key;
	POSITION pos = cell->m_feaMap.GetStartPosition(); //value that came to change.

	while (pos != NULL)
	{
		R_FeatureRecord* fe = new R_FeatureRecord();
		cell->m_feaMap.GetNextAssoc(pos, key, fe);
		auto UpdateName = fe->m_frid.m_name.GetName();
		auto mission = fe->m_frid.m_ruin;

		if (mission == 1)
		{
			InsertFeatureRecord(UpdateName, fe);
		}
		else
		{
			R_FeatureRecord* values = new R_FeatureRecord();
			auto isValues = m_feaMap.Lookup(UpdateName, values);

			if (isValues == false)
			{
				continue;
			}


			if (mission == 2)
			{
				RemoveFeatureRecord(UpdateName);
			}
			else if (mission == 3) //change
			{
				//FRID
				values->m_frid.m_name.RCNM = fe->m_frid.m_name.RCNM;
				values->m_frid.m_name.RCID = fe->m_frid.m_name.RCID;
				values->m_frid.m_nftc = fe->m_frid.m_nftc;
				values->m_frid.m_rver = fe->m_frid.m_rver;

				//FOID
				values->m_foid.AGEN = fe->m_foid.AGEN;
				values->m_foid.FIDN = fe->m_foid.FIDN;
				values->m_foid.FIDS = fe->m_foid.FIDS;

				//need ATTR
				UpdateAttrRecord(fe->m_attr, values->m_attr);

				//INAS
				UpdateINASRecord(fe->m_inas, values->m_inas);


				//need change SPAS 
				auto spasbase = values->m_spas;
				for (F_SPAS* f_spas : fe->m_spas)
				{
					auto it = std::find(spasbase.begin(), spasbase.end(), f_spas);
					if (it == spasbase.end()) //has value
					{
						continue;
					}

					int i = 0;
					for (auto spas : f_spas->m_arr)
					{
						int mission = spas->m_saui;

						auto it = values->m_spas.begin();
						std::advance(it, i);
						auto Basearr = (*it)->m_arr;

						if (mission == 1)
						{
							Basearr.push_back(spas);
						}

						else if (mission == 2)
						{
							auto isArr = std::find(Basearr.begin(), Basearr.end(), spas);
							if (isArr != Basearr.end())
							{
								Basearr.erase(isArr);
							}
						}
						i++;
					}
				}

				//All the FASC changes are in the form of a list.
				for (auto f_fasc : fe->m_fasc)
				{
					int mission = f_fasc->m_faui;
					if (1 == mission)//add
					{
						values->m_fasc.push_back(f_fasc);
						continue;
					}


					else if (2 == mission)//delete
					{
						auto it = std::find(values->m_fasc.begin(), values->m_fasc.end(), f_fasc); //find value than delete
						if (it != values->m_fasc.end())
						{
							values->m_fasc.erase(it);
						}
					}

					else if (3 == mission) //modify
					{
						for (ATTR* attr : f_fasc->m_arr)
						{
							int attriMission = attr->m_atin;
							auto it = std::find(values->m_fasc.begin(), values->m_fasc.end(), f_fasc);


							if (1 == attriMission) //insert
							{
								(*it)->m_arr.push_back(attr);
							}

							else if (2 == attriMission) //delete
							{
								auto itor = std::find((*it)->m_arr.begin(), (*it)->m_arr.end(), attr);
								(*it)->m_arr.erase(itor);
							}

							else if (3 == attriMission) //modify
							{
								auto itor = std::find((*it)->m_arr.begin(), (*it)->m_arr.end(), attr);
								*itor = attr;
							}
						}
					}
				}

				int i = 0;
				for (auto f_mask : fe->m_mask)
				{
					for (auto mask : f_mask->listMask)
					{
						auto Key = (__int64)mask->m_name.GetName();
						auto mission = mask->m_muin;

						auto it = values->m_mask.begin();
						std::advance(it, i);

						if (1 == mission) //insert
						{
							(*it)->AddMask(mask);
						}
						else //delete
						{
							(*it)->DeleteMask(mask->m_name);
						}
					}
				}
			}
		}
	}

	return true;
}

void S101Cell::InitCurveSuppression()
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		auto feature = *i;

		if (feature->geometry->GetType() == SGeometryType::CompositeCurve)
		{
			auto compositeCurve = (SCompositeCurve*)feature->geometry;
			compositeCurve->setSuppress(false);

			//int curveCnt = compositeCurve->GetCurveCount();
			//for (int j = 0; j < curveCnt; j++)
			//{
			//	auto c = compositeCurve->GetCurve(j);
			//	c->SetSuppress(false);
			//}
		}
		else if (feature->geometry->GetType() == SGeometryType::Surface)
		{
			auto surface = (SSurface*)feature->geometry;
			surface->setSuppress(false);
		}
		else if (feature->geometry->GetType() == SGeometryType::Curve)
		{
			auto curve = (SCurve*)feature->geometry;
			curve->SetSuppress(false);
		}
	}
}

bool S101Cell::Save(std::wstring path)
{
	auto ext = LibMFCUtil::GetExtension(path.c_str());
	if (ext.CompareNoCase(L"gml") != 0)
	{
		return SaveAsENC(path);
	}
	else
	{
		return SaveAsGML(path);
	}

	return false;
}

bool S101Cell::SaveAsENC(std::wstring path)
{
	CString filePath(path.c_str());

	CFile file;
	file.Open(filePath, CFile::modeCreate | CFile::modeWrite);

	// DDR
	R_DDR ddr;
	ddr.f_FieldControlField.AddTagPair("DSID", "DSSI");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::DSSI));

	if (m_dsgir.m_atcs)
	{
		ddr.f_FieldControlField.AddTagPair("DSID", "ATCS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::ATCS));
	}

	if (m_dsgir.m_itcs)
	{
		ddr.f_FieldControlField.AddTagPair("DSID", "ITCS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::ITCS));
	}

	if (m_dsgir.m_ftcs)
	{
		ddr.f_FieldControlField.AddTagPair("DSID", "FTCS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FTCS));
	}

	if (m_dsgir.m_iacs)
	{
		ddr.f_FieldControlField.AddTagPair("DSID", "IACS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::IACS));
	}

	if (m_dsgir.m_facs)
	{
		ddr.f_FieldControlField.AddTagPair("DSID", "FACS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FACS));
	}

	if (m_dsgir.m_arcs)
	{
		ddr.f_FieldControlField.AddTagPair("DSID", "ARCS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::DSID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::ARCS));
	}

	ddr.f_FieldControlField.AddTagPair("CSID", "CRSH");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::CSID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::CRSH));

	if (m_dscrs.m_csax)
	{
		ddr.f_FieldControlField.AddTagPair("CRSH", "CSAX");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::CRSH));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::CSAX));
	}

	if (m_dscrs.m_vdat)
	{
		ddr.f_FieldControlField.AddTagPair("CRSH", "VDAT");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::CRSH));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::VDAT));
	}

	if (InformationRecordHasAttributeField())
	{
		ddr.f_FieldControlField.AddTagPair("IRID", "ATTR");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::IRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::ATTR));
	}

	if (InformationRecordHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("IRID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::IRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	if (PointRecordHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("PRID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::PRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	ddr.f_FieldControlField.AddTagPair("PRID", "C2IT");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::PRID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::C2IT));

	if (MultiPointRecordHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("MRID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::MRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	ddr.f_FieldControlField.AddTagPair("MRID", "C3IL");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::MRID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::C3IL));

	if (CurveRecordHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("CRID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::CRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	ddr.f_FieldControlField.AddTagPair("CRID", "PTAS");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::CRID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::PTAS));

	ddr.f_FieldControlField.AddTagPair("CRID", "SEGH");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::CRID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::SEGH));

	ddr.f_FieldControlField.AddTagPair("SEGH", "C2IL");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::SEGH));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::C2IL));

	if (CompositeCurveHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("CCID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::CCID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	ddr.f_FieldControlField.AddTagPair("CCID", "CUCO");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::CCID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::CUCO));

	if (SurfaceRecordHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("SRID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::SRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	ddr.f_FieldControlField.AddTagPair("SRID", "RIAS");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::SRID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::RIAS));

	ddr.f_FieldControlField.AddTagPair("FRID", "FOID");
	ddr.AddDDF(F_DataDescriptiveField(DDFType::FRID));
	ddr.AddDDF(F_DataDescriptiveField(DDFType::FOID));

	if (FeatureRecordHasAttributeField())
	{
		ddr.f_FieldControlField.AddTagPair("FRID", "ATTR");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::ATTR));
	}

	if (FeatureRecordHasInformationAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("FRID", "INAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::INAS));
	}

	if (FeatureRecordHasSpatialAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("FRID", "SPAS");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::SPAS));
	}

	if (FeatureRecordHasFeatureAssociationField())
	{
		ddr.f_FieldControlField.AddTagPair("FRID", "FASC");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FASC));
	}

	if (FeatureRecordHasMaskedSpatialTypeField())
	{
		ddr.f_FieldControlField.AddTagPair("FRID", "MASK");
		ddr.AddDDF(F_DataDescriptiveField(DDFType::FRID));
		ddr.AddDDF(F_DataDescriptiveField(DDFType::MASK));
	}

	ddr.WriteRecord(&file);

	m_dsgir.m_dssi.SetNumberOfInformationTypeRecords((int)vecInformation.size());
	m_dsgir.m_dssi.SetNumberOfPointRecords((int)vecPoint.size());
	m_dsgir.m_dssi.SetNumberOfMultiPointRecords((int)vecMultiPoint.size());
	m_dsgir.m_dssi.SetNumberOfCurveRecords((int)vecCurve.size());
	m_dsgir.m_dssi.SetNumberOfCompositeCurveRecords((int)vecComposite.size());
	m_dsgir.m_dssi.SetNumberOfSurfaceRecords((int)vecSurface.size());
	m_dsgir.m_dssi.SetNumberOfFeatureTypeRecords((int)vecFeature.size());

	m_dsgir.WriteRecord(&file);
	m_dscrs.WriteRecord(&file);

	for (auto i = vecInformation.begin(); i != vecInformation.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	for (auto i = vecPoint.begin(); i != vecPoint.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	for (auto i = vecMultiPoint.begin(); i != vecMultiPoint.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	for (auto i = vecCurve.begin(); i != vecCurve.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	for (auto i = vecComposite.begin(); i != vecComposite.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	for (auto i = vecSurface.begin(); i != vecSurface.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		(*i)->WriteRecord(&file);
	}

	return true;
}

bool S101Cell::SaveAsGML(std::wstring path)
{
	pugi::xml_document doc;

	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";

	auto root = doc.append_child("S101:Dataset");
	root.append_attribute("xmlns:xsi").set_value("http://www.w3.org/2001/XMLSchema-instance");
	root.append_attribute("xmlns:xlink").set_value("http://www.w3.org/1999/xlink");
	root.append_attribute("xmlns:gml").set_value("http://www.opengis.net/gml/3.2");
	root.append_attribute("xmlns:S100").set_value("http://www.iho.int/s100gml/5.0");
	root.append_attribute("xmlns:S101").set_value("http://www.iho.int/S101/gml/cs0/1.0");
	root.append_attribute("xmlns").set_value("http://www.iho.int/S101/gml/cs0/1.0");
	root.append_attribute("gml:id").set_value("ds");

	if (m_pLayer)
	{
		S100GML::Envelop env(m_pLayer->m_mbr);
		env.Write(root);
	}

	GetDatasetIdentificationInformation().Write(root);

	SavePoint(root);
	SaveMultiPoint(root);
	SaveCurve(root);
	SaveCompositeCurve(root);
	SaveSurface(root);

	SaveMembers(root);

	doc.save_file(path.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);

	return true;
}

bool S101Cell::SavePoint(pugi::xml_node& root)
{

	for (auto i = vecPoint.begin(); i != vecPoint.end(); i++)
	{
		auto record = *i;
		auto node_Point = root.append_child("S100:Point");
		node_Point.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
		node_Point.append_attribute("gml:id").set_value(record->GetRCIDasString("p").c_str());
		auto nodePos = node_Point.append_child("gml:pos");

		SPoint pt;
		GetFullSpatialData(*i, &pt);

		nodePos.append_child(pugi::node_pcdata).set_value(pt.ToString().c_str());
	}

	return true;
}

bool S101Cell::SaveMultiPoint(pugi::xml_node& root)
{
	for (auto i = vecMultiPoint.begin(); i != vecMultiPoint.end(); i++)
	{
		auto record = *i;
		auto curNode = root.append_child("S100:MultiPoint");
		curNode.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
		curNode.append_attribute("gml:id").set_value(record->GetRCIDasString("mp").c_str());
		auto nodeMembers = curNode.append_child("gml:pointMembers");

		SMultiPoint mp;
		GetFullSpatialData(*i, &mp);

		for (int j = 0; j < mp.GetNumPoints(); j++)
		{
			auto nodePoint = nodeMembers.append_child("gml:Point");

			auto id = record->GetRCIDasString("mp") + "_p" + std::to_string(j);
			nodePoint.append_attribute("gml:id").set_value(id.c_str());
			auto nodePos = nodePoint.append_child("gml:pos");

			nodePos.append_child(pugi::node_pcdata).set_value(mp.m_pPoints.at(j).ToString().c_str());
		}
	}

	return true;
}

bool S101Cell::SaveCurve(pugi::xml_node& root)
{
	for (auto i = vecCurve.begin(); i != vecCurve.end(); i++)
	{
		auto record = *i;
		auto curNode = root.append_child("S100:Curve");
		curNode.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
		curNode.append_attribute("gml:id").set_value(record->GetRCIDasString("c").c_str());
		auto nodePosList = curNode.append_child("gml:segments").append_child("gml:LineStringSegment").append_child("gml:posList");

		SCurve curve;
		GetFullSpatialData(*i, &curve);

		nodePosList.append_child(pugi::node_pcdata).set_value(curve.ToString().c_str());
	}

	return true;
}

bool S101Cell::SaveCompositeCurve(pugi::xml_node& root)
{
	for (auto i = vecComposite.begin(); i != vecComposite.end(); i++)
	{
		auto record = *i;
		auto curNode = root.append_child("S100:CompositeCurve");

		auto node_prevSibling = curNode.previous_sibling();

		curNode.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
		curNode.append_attribute("gml:id").set_value(record->GetRCIDasString("cc").c_str());

		for (auto j = record->m_cuco.begin(); j != record->m_cuco.end(); j++)
		{
			auto f_cuco = (*j);
			for (auto k = f_cuco->m_arr.begin(); k != f_cuco->m_arr.end(); k++)
			{
				auto node_CurveMember = curNode.append_child("gml:curveMember");

				auto cuco = *k;

				std::string prefix;
				if (cuco->IsCurve())
				{
					prefix = "c";
				}
				else
				{
					prefix = "cc";
				}

				if (cuco->m_ornt == 1) // Forward
				{
					node_CurveMember.append_attribute("xlink:href").set_value(cuco->m_name.GetRCIDasString("#" + prefix).c_str());
				}
				else if (cuco->m_ornt == 2) // Reverse
				{
					auto ocID = cuco->m_name.GetRCIDasString("o" + prefix);
					auto find = HasOrientableCurve(root, ocID);
					if (!find)
					{
						AddOrientableCurve(root, node_prevSibling, ocID, cuco->m_name.GetRCIDasString(prefix));
					}

					node_CurveMember.append_attribute("xlink:href").set_value(cuco->m_name.GetRCIDasString("#o" + prefix).c_str());
				}
			}
		}
	}

	return true;
}

bool S101Cell::SaveSurface(pugi::xml_node& root)
{
	pugi::xml_node& rootOfOrientableCurve = root;
	for (auto i = vecSurface.begin(); i != vecSurface.end(); i++)
	{
		auto record = *i;

		auto node_Surface = root.append_child("S100:Surface");

		auto node_prevSibling = node_Surface.previous_sibling();

		node_Surface.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
		node_Surface.append_attribute("gml:id").set_value(record->GetRCIDasString("s").c_str());

		auto node_PolygonPatch = node_Surface.append_child("gml:patches")
			.append_child("gml:PolygonPatch");

		for (auto j = record->m_rias.begin(); j != record->m_rias.end(); j++)
		{
			auto f_rias = (*j);
			for (auto k = f_rias->m_arr.begin(); k != f_rias->m_arr.end(); k++)
			{
				auto rias = *k;
				std::string usageName;
				if (rias->IsExterior())
				{
					usageName = "gml:exterior";
				}
				else
				{
					usageName = "gml:interior";
				}

				std::string curvePrefix;
				if (rias->IsCurve())
				{
					curvePrefix = "c";
				}
				else
				{
					curvePrefix = "cc";
				}

				std::string refCurveID;
				if (rias->IsReverse())
				{
					auto ocID = rias->m_name.GetRCIDasString("o" + curvePrefix);
					if (!HasOrientableCurve(root, ocID))
					{
						AddOrientableCurve(root, node_Surface, ocID, rias->m_name.GetRCIDasString(curvePrefix));
					}

					refCurveID = "#" + ocID;
				}
				else
				{
					refCurveID = "#" + curvePrefix + rias->m_name.GetRCIDasString();
				}

				auto node_CurveMember = node_PolygonPatch.append_child(usageName.c_str())
					.append_child("gml:Ring")
					.append_child("gml:curveMember")
					.append_attribute("xlink:href").set_value(refCurveID.c_str());
			}
		}
	}

	return true;
}

bool S101Cell::SaveMembers(pugi::xml_node& root)
{
	auto node_members = root.append_child("members");

	SaveInfomation(node_members);
	SaveFeature(node_members);
	return true;
}

bool S101Cell::SaveInfomation(pugi::xml_node& root)
{
	for (auto i = vecInformation.begin(); i != vecInformation.end(); i++)
	{
		auto info = *i;

		auto code = pugi::as_utf8(GetInformationTypeCodeByID(info->GetRCID()));
		auto node_info = root.append_child(code.c_str());
		node_info.append_attribute("gml:id").set_value(info->GetRCIDAsString("i").c_str());

		auto attributes = info->GetAllAttributes();

		std::vector<pugi::xml_node> attributeNodes;
		attributeNodes.push_back(node_info);

		SaveAttribute(node_info, attributes);

		auto inas = info->GetAllInformationAssociations();
		SaveInformationAssociation(node_info, inas);
	}

	return true;
}

bool S101Cell::SaveFeature(pugi::xml_node& root)
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		auto feature = *i;

		auto code = pugi::as_utf8(GetFeatureTypeCodeByID(feature->GetRCID()));
		auto node_feature = root.append_child(code.c_str());
		node_feature.append_attribute("gml:id").set_value(feature->GetRCIDAsString("f").c_str());

		auto attributes = feature->GetAllAttributes();

		SaveAttribute(node_feature, attributes);

		auto inas = feature->GetAllInformationAssociations();
		SaveInformationAssociation(node_feature, inas);

		auto fasc = feature->GetAllFeatureAssociations();
		SaveFeatureAssociation(node_feature, fasc);

		auto spas = feature->GetSPAS();
		SaveGeometry(node_feature, spas);
	}

	return true;
}

bool S101Cell::SaveAttribute(pugi::xml_node& root, std::vector<ATTR*> attributes)
{
	auto fc = GetFC();
	if (!fc)
	{
		return false;
	}

	std::vector<pugi::xml_node> attributeNodes;
	attributeNodes.push_back(root);

	for (auto j = attributes.begin(); j != attributes.end(); j++)
	{
		auto attr = (*j);

		auto code = pugi::as_utf8(m_dsgir.GetAttributeCode(attr->m_natc));
		auto sa = fc->GetSimpleAttribute(code);
		auto ca = fc->GetComplexAttribute(code);

		auto parentNode = attributeNodes[attr->m_paix];

		if (parentNode == pugi::xml_node(NULL))
		{
			attributeNodes.push_back(pugi::xml_node(NULL));
			continue;
		}

		if (sa)
		{
			std::string value;

			auto type = sa->GetValueType();
			if (type == FCD::S100_CD_AttributeValueType::enumeration)
			{
				auto listedValue = sa->GetListedValue(_ttoi(attr->m_atvl));
				if (listedValue)
				{
					value = pugi::as_utf8(listedValue->GetLabel());
				}
				else
				{
					value = "";
				}
			}
			else
			{
				value = pugi::as_utf8(attr->m_atvl);
			}

			auto sa_node = SaveSimpleAttribute(parentNode, code, value);
			attributeNodes.push_back(sa_node);
		}
		else if (ca)
		{
			auto ca_node = SaveComplexAttribute(parentNode, code);
			attributeNodes.push_back(ca_node);
		}
		else
		{
			attributeNodes.push_back(pugi::xml_node(NULL));
		}
	}

	return true;
}

bool S101Cell::SaveInformationAssociation(pugi::xml_node& root, std::vector<F_INAS*> inas)
{
	auto fc = GetFC();
	if (nullptr == fc)
	{
		return false;
	}

	for (auto i = inas.begin(); i != inas.end(); i++)
	{
		auto curINAS = (*i);
		auto wIACode = std::wstring(m_dsgir.GetInformationAssociationCode(curINAS->m_niac));
		auto wRoleCode = std::wstring(m_dsgir.GetAssociationRoleCode(curINAS->m_narc));
		auto ia = fc->GetInformationAssociation(wIACode);
		auto role = fc->GetRole(wRoleCode);

		if (ia && role)
		{
			auto node_ia = root.append_child(role->GetCode().c_str());
			node_ia.append_attribute("xlink:href").set_value(curINAS->m_name.GetRCIDasString("#i").c_str());
			node_ia.append_attribute("xlink:arcrole").set_value(std::string("http://www.iho.net/S-101/roles/" + role->GetCode()).c_str());
			node_ia.append_attribute("xlink:title").set_value(ia->GetCode().c_str());
		}
	}

	return true;
}

bool S101Cell::SaveFeatureAssociation(pugi::xml_node& root, std::vector<F_FASC*> fasc)
{
	auto fc = GetFC();
	if (nullptr == fc)
	{
		return false;
	}

	for (auto i = fasc.begin(); i != fasc.end(); i++)
	{
		auto curFASC = (*i);
		auto wFACode = std::wstring(m_dsgir.GetFeatureAssociationCode(curFASC->m_nfac));
		auto wRoleCode = std::wstring(m_dsgir.GetAssociationRoleCode(curFASC->m_narc));
		auto fa = fc->GetFeatureAssociation(wFACode);
		auto role = fc->GetRole(wRoleCode);

		if (fa && role)
		{
			auto node_fa = root.append_child(role->GetCode().c_str());
			node_fa.append_attribute("xlink:href").set_value(curFASC->m_name.GetRCIDasString("#f").c_str());
			node_fa.append_attribute("xlink:arcrole").set_value(std::string("http://www.iho.net/S-101/roles/" + role->GetCode()).c_str());
			node_fa.append_attribute("xlink:title").set_value(fa->GetCode().c_str());
		}
	}

	return true;
}

bool S101Cell::SaveGeometry(pugi::xml_node& root, SPAS* spas)
{
	if (spas)
	{
		auto node_geometry = root.append_child("geometry");
		if (110 == spas->m_name.RCNM)
		{
			node_geometry.append_child("S100:pointProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#p").c_str());
		}
		else if (115 == spas->m_name.RCNM)
		{
			node_geometry.append_child("S100:multiPointProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#mp").c_str());
		}
		else if (120 == spas->m_name.RCNM)
		{
			if (2 == spas->m_ornt)
			{
				node_geometry.append_child("S100:curveProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#oc").c_str());
			}
			else
			{
				node_geometry.append_child("S100:curveProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#c").c_str());
			}
		}
		else if (125 == spas->m_name.RCNM)
		{
			if (2 == spas->m_ornt)
			{
				node_geometry.append_child("S100:compositeCurveProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#occ").c_str());
			}
			else
			{
				node_geometry.append_child("S100:compositeCurveProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#cc").c_str());
			}
		}
		else if (130 == spas->m_name.RCNM)
		{
			node_geometry.append_child("S100:surfaceProperty").append_attribute("xlink:href").set_value(spas->m_name.GetRCIDasString("#s").c_str());
		}
	}

	return true;
}

pugi::xml_node S101Cell::SaveSimpleAttribute(pugi::xml_node root, std::string code, std::string value)
{
	auto sa_node = root.append_child(code.c_str());
	sa_node.append_child(pugi::node_pcdata).set_value(value.c_str());
	return sa_node;
}

pugi::xml_node S101Cell::SaveComplexAttribute(pugi::xml_node root, std::string code)
{
	return root.append_child(code.c_str());
}

bool S101Cell::HasOrientableCurve(pugi::xml_node& root, std::string id)
{
	auto findNode = root.find_child_by_attribute("S100:OrientableCurve", "gml:id", id.c_str());

	if (findNode == nullptr)
	{
		return false;
	}

	return true;
}

void S101Cell::AddOrientableCurve(pugi::xml_node& root, pugi::xml_node& prevNode, std::string odID, std::string refID)
{
	auto node_oc = root.insert_child_after("S100:OrientableCurve", prevNode);
	node_oc.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");

	node_oc.append_attribute("gml:id").set_value(odID.c_str());
	node_oc.append_attribute("orientation").set_value("-");

	auto node_bc = node_oc.append_child("gml:baseCurve");
	node_bc.append_attribute("xlink:href").set_value(std::string("#" + refID).c_str());
}

bool S101Cell::InformationRecordHasAttributeField()
{
	for (auto i = vecInformation.begin(); i != vecInformation.end(); i++)
	{
		if ((*i)->m_attr.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::InformationRecordHasInformationAssociationField()
{
	for (auto i = vecInformation.begin(); i != vecInformation.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::PointRecordHasInformationAssociationField()
{
	for (auto i = vecPoint.begin(); i != vecPoint.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::MultiPointRecordHasInformationAssociationField()
{
	for (auto i = vecMultiPoint.begin(); i != vecMultiPoint.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::CurveRecordHasInformationAssociationField()
{
	for (auto i = vecCurve.begin(); i != vecCurve.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::CompositeCurveHasInformationAssociationField()
{
	for (auto i = vecComposite.begin(); i != vecComposite.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::SurfaceRecordHasInformationAssociationField()
{
	for (auto i = vecSurface.begin(); i != vecSurface.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::FeatureRecordHasAttributeField()
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		if ((*i)->m_attr.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::FeatureRecordHasInformationAssociationField()
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		if ((*i)->m_inas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::FeatureRecordHasSpatialAssociationField()
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		if ((*i)->m_spas.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::FeatureRecordHasFeatureAssociationField()
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		if ((*i)->m_fasc.size() > 0)
		{
			return true;
		}
	}

	return false;
}

bool S101Cell::FeatureRecordHasMaskedSpatialTypeField()
{
	for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
	{
		if ((*i)->m_mask.size() > 0)
		{
			return true;
		}
	}

	return false;
}

GF::ObjectType* S101Cell::GetObjectType(int type, std::string id)
{
	if (type == 1)
	{
		return GetFeatureType(id);
	}
	else if (type == 2)
	{
		return GetInformationType(id);
	}

	return nullptr;
}


std::wstring S101Cell::GetFeatureTypeCodeByID(std::wstring id)
{
	int rcid = std::stoi(id);
	return GetFeatureTypeCodeByID(rcid);
}

std::wstring S101Cell::GetFeatureTypeCodeByID(std::string id)
{
	int rcid = std::stoi(id);
	return GetFeatureTypeCodeByID(rcid);
}

std::wstring S101Cell::GetFeatureTypeCodeByID(int id)
{
	RecordName rn(100, id);
	auto fe = GetFeatureRecord(rn.GetName());
	if (fe)
	{
		return std::wstring(m_dsgir.GetFeatureCode(fe->GetNumericCode()));
	}

	return L"";
}

std::wstring S101Cell::GetInformationTypeCodeByID(std::wstring id)
{
	auto key = std::stoi(id);
	return GetInformationTypeCodeByID(key);
}

std::wstring S101Cell::GetInformationTypeCodeByID(std::string id)
{
	return GetInformationTypeCodeByID(pugi::as_wide(id));
}

std::wstring S101Cell::GetInformationTypeCodeByID(int id)
{
	RecordName rn(150, id);
	R_InformationRecord* ir = nullptr;
	if (TRUE == m_infMap.Lookup(rn.GetName(), ir))
	{
		if (ir)
		{
			return std::wstring(m_dsgir.GetInformationCode(ir->GetNumericCode()));
		}
	}

	return L"";
}

int S101Cell::GetFeatureCount()
{
	return GetCount_FeatureTypeRecord();
}

int S101Cell::GetInformationCount()
{
	return GetCount_InformationRecord();
}

GF::FeatureType* S101Cell::GetFeatureType(std::string id)
{
	return GetFeatureRecord(id);
}

GF::FeatureType* S101Cell::GetFeatureTypeByIndex(int index)
{
	return GetFeatureRecordByIndex(index);
}

GF::InformationType* S101Cell::GetInformationType(std::string id)
{
	return GetInformationRecord(id);
}

GF::InformationType* S101Cell::GetInformationTypeByIndex(int index)
{
	return GetInformationRecordByIndex(index);
}

std::string S101Cell::GetFeatureAssociationCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetFeatureRelationCount())
	{
		return "";
	}

	auto fr = (R_FeatureRecord*)featureType;

	auto i = fr->m_fasc.begin();
	std::advance(i, index);
	return pugi::as_utf8(std::wstring(m_dsgir.GetFeatureAssociationCode((*i)->m_nfac)).c_str());
}

std::string S101Cell::GetFeatureAssociationRoleCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetFeatureRelationCount())
	{
		return "";
	}

	auto fr = (R_FeatureRecord*)featureType;

	auto i = fr->m_fasc.begin();
	std::advance(i, index);
	return pugi::as_utf8(std::wstring(m_dsgir.GetAssociationRoleCode((*i)->m_narc)).c_str());
}

std::string S101Cell::GetInformationAssociationCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetInformationRelationCount())
	{
		return "";
	}

	auto fr = (R_FeatureRecord*)featureType;

	auto i = fr->m_inas.begin();
	std::advance(i, index);
	return pugi::as_utf8(std::wstring(m_dsgir.GetInformationAssociationCode((*i)->m_niac)).c_str());
}

std::string S101Cell::GetInformationAssociationRoleCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetInformationRelationCount())
	{
		return "";
	}

	auto fr = (R_FeatureRecord*)featureType;

	auto i = fr->m_inas.begin();
	std::advance(i, index);
	return pugi::as_utf8(std::wstring(m_dsgir.GetAssociationRoleCode((*i)->m_narc)).c_str());
}

std::string S101Cell::GetInformationAssociationCode(GF::InformationType* informationType, int index)
{
	if (index < 0 || index >= informationType->GetInformationRelationCount())
	{
		return "";
	}

	auto fr = (R_InformationRecord*)informationType;

	auto i = fr->m_inas.begin();
	std::advance(i, index);
	return pugi::as_utf8(std::wstring(m_dsgir.GetInformationAssociationCode((*i)->m_niac)).c_str());
}

std::string S101Cell::GetInformationAssociationRoleCode(GF::InformationType* informationType, int index)
{
	if (index < 0 || index >= informationType->GetInformationRelationCount())
	{
		return "";
	}

	auto fr = (R_InformationRecord*)informationType;

	auto i = fr->m_inas.begin();
	std::advance(i, index);
	return pugi::as_utf8(std::wstring(m_dsgir.GetAssociationRoleCode((*i)->m_narc)).c_str());
}

//std::string S101Cell::GetObjectAttributeCode(int type, std::string id, int index)
//{
//	if (type == 1)
//	{
//		return GetFeatureAttributeCode(id, index);
//	}
//	else if (type == 2)
//	{
//		return GetInformationAttributeCode(id, index);
//	}
//
//	return "";
//}

//int S101Cell::GetFeatureAttributeCount(std::string id)
//{
//	auto object = GetFeatureType(id);
//	if (object)
//	{
//		return object->GetAttributeCount();
//	}
//
//	return 0;
//}

std::string S101Cell::GetFeatureAttributeCode(std::string id, int index)
{
	auto object = (R_FeatureRecord*)GetFeatureType(id);
	if (object)
	{
		auto attr = object->GetAllAttributes();

		if (index >= 0 && index < attr.size())
		{
			auto numericCode = attr.at(index)->m_natc;
			auto code = m_dsgir.GetAttributeCode(numericCode);
			return pugi::as_utf8(code);
		}
	}

	return "";
}

//int S101Cell::GetInformationAttributeCount(std::string id)
//{
//	auto object = GetInformationType(id);
//	if (object)
//	{
//		return object->GetAttributeCount();
//	}
//
//	return 0;
//}

std::string S101Cell::GetInformationAttributeCode(std::string id, int index)
{
	auto object = (R_InformationRecord*)GetInformationType(id);
	if (object)
	{
		auto attr = object->GetAllAttributes();

		if (index >= 0 && index < attr.size())
		{
			auto numericCode = attr.at(index)->m_natc;
			auto code = m_dsgir.GetAttributeCode(numericCode);
			return pugi::as_utf8(code);
		}
	}

	return "";
}

int S101Cell::CoordinateMultiplicationFactorForX()
{
	return m_dsgir.m_dssi.m_cmfx;
}

int S101Cell::CoordinateMultiplicationFactorForY()
{
	return m_dsgir.m_dssi.m_cmfy;
}

int S101Cell::CoordinateMultiplicationFactorForZ()
{
	return m_dsgir.m_dssi.m_cmfz;
}

std::wstring S101Cell::GetChartName()
{
	// return filename 
	// ex : 101GB005X01SW.000
	return std::wstring(GetFileName());
}

std::wstring S101Cell::GetEditionNumberAsWstring()
{
	CString Ened = m_dsgir.m_dsid.m_ened;
	auto index = Ened.Find(L".");
	CString result;
	AfxExtractSubString(result, Ened, 0, '.');

	// The ENED of the DSID contains values in the form of n.m. such as 1.0, 1.1, etc.
	// n is Edition Number.
	// m is Update Number.
	// I'll return the Edition Number here.
	// e.g. If it's 1.4, return 1
	return std::wstring(result);
}

std::string S101Cell::GetUpdateNumber()
{
	return pugi::as_utf8(GetUpdateNumberAsWstring());
}

std::wstring S101Cell::GetUpdateNumberAsWstring()
{
	auto Ened = m_dsgir.m_dsid.m_dsed;

	CString result;
	AfxExtractSubString(result, Ened, 1, '.');

	// The ENED of the DSID contains values in the form of n.m. such as 1.0, 1.1, etc.
	// n is Edition Number.
	// m is Update Number.
	// I'll return the Edition Number here.
	// e.g. If it's 1.4, return 4
	return std::wstring(result);
}

std::wstring S101Cell::GetIssueDateAsWstring()
{
	CString Dsrd = m_dsgir.m_dsid.m_dsrd;

	// Output DSRD of DSID.
	// DSRD is in the form of YYYMMDD (e.g., 20210324).
	// Return Example: 2012-03-24
	auto index = Dsrd.Find(_T("-"));
	if (index == -1)
	{
		CString year = Dsrd.Left(4);
		CString month = Dsrd.Mid(4, 2);
		CString day = Dsrd.Right(2);

		std::wstring result = year + L"-" + month + L"-" + day;
		return result;
	}
	else
	{
		return std::wstring(Dsrd);
	}
}

std::vector<std::string> S101Cell::Query(MBR mbr)
{
	return {};
}

std::vector<std::string> S101Cell::QueryToSurface(MBR mbr)
{
	return {};
}

std::vector<std::string> S101Cell::QueryToCurve(MBR mbr)
{
	return {};
}

std::vector<std::string> S101Cell::QueryToPoint(MBR mbr)
{
	return {};
}

std::vector<std::string> S101Cell::QueryToMultiPoint(MBR mbr)
{
	return {};
}

S100GML::DatasetIdentificationInformation S101Cell::GetDatasetIdentificationInformation()
{
	S100GML::DatasetIdentificationInformation result;

	result.encodingSpecification = "S-100 Part 10b";
	result.encodingSpecificationEdition = "1.0";
	result.productIdentifier = "INT.IHO.S-101.1.0";
	result.productEdition = "1.0.0";
	result.applicationProfile = "1";
	result.datasetFileIdentifier = LibMFCUtil::WStringToString(std::wstring(GetFileName()));
	result.datasetTitle = LibMFCUtil::WStringToString(std::wstring(LibMFCUtil::GetFileName(GetFilePath())));

	if (m_dsgir.m_dsid.m_dsrd.GetLength() >= 8)
	{
		CString dsrd = m_dsgir.m_dsid.m_dsrd;

		std::string yyyy = pugi::as_utf8(dsrd.Left(4));
		std::string mm = pugi::as_utf8(dsrd.Mid(4, 2));
		std::string dd = pugi::as_utf8(dsrd.Mid(6, 2));

		result.datasetReferenceDate = yyyy + "-" + mm + "-" + dd;
	}

	result.datasetLanguage = "eng";
	result.datasetTopicCategory.push_back("oceans");
	result.datasetTopicCategory.push_back("transportation");
	result.datasetPurpose = "base";
	result.updateNumber = GetUpdateNumber();

	return result;
}

void S101Cell::WritePointRecord(pugi::xml_node& node, R_PointRecord* record)
{
	auto curNode = node.append_child("S100:Point");
	curNode.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
	curNode.append_attribute("gml:id").set_value("p" + record->GetRCID());

	//GetPoint

	//curNode.append_child("gml:pos").append_child(pugi::node_pcdata).set_value()
}

bool S101Cell::ConvertFromS101GML(S10XGML* gml)
{
	S101Creator creator(this);

	ConvertInformationsFromS101GML(gml, &creator);
	ConvertFeaturesFromS101GML(gml, &creator);
	ConvertGeometriesFromS101GML(gml);

	return true;
}

bool S101Cell::ConvertFromS101GML(S101Creator* creator, R_FeatureRecord* featureRecord, GF::FeatureType* featureType)
{
	auto cntAttr = featureType->GetAttributeCount();
	for (int i = 0; i < cntAttr; i++)
	{
		auto attribute = featureType->GetAttribute(i);
		auto code = attribute->GetCode();
		if (attribute->IsSimple())
		{
			ConvertFromS101GML(creator, featureRecord, (GF::SimpleAttributeType*)attribute);
		}
		else
		{
			ConvertFromS101GML(creator, featureRecord, nullptr, (GF::ComplexAttributeType*)attribute);
		}
	}

	return true;
}

bool S101Cell::ConvertFromS101GML(S101Creator* creator, R_FeatureRecord* featureRecord, GF::SimpleAttributeType* simpleAttribute)
{
	auto fc = GetFC();
	auto sa = fc->GetSimpleAttribute(simpleAttribute->GetCode());
	if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
	{
		auto listedValue = sa->GetListedValue(simpleAttribute->GetValue());
		if (listedValue)
		{
			creator->AddSimpleAttribute(featureRecord, simpleAttribute->GetCode(), std::to_string(listedValue->GetCode()));
			return true;
		}
	}
	else
	{
		creator->AddSimpleAttribute(featureRecord, simpleAttribute->GetCode(), simpleAttribute->GetValue());
		return true;
	}

	return false;
}

bool S101Cell::ConvertFromS101GML(S101Creator* creator, R_FeatureRecord* featureRecord, ATTR* parentATTR, GF::ComplexAttributeType* complexAttribute)
{
	auto fc = GetFC();
	ATTR* addedCA = nullptr;

	if (parentATTR == nullptr)
	{
		addedCA = creator->AddComplexAttribute(featureRecord, complexAttribute->GetCode());
	}
	else
	{
		addedCA = creator->AddComplexAttribute(featureRecord, parentATTR, complexAttribute->GetCode());
	}

	int cnt = complexAttribute->GetSubAttributeCount();

	for (int i = 0; i < cnt; i++)
	{
		auto subAttribute = complexAttribute->GetSubAttribute(i);
		if (subAttribute->IsSimple())
		{
			auto sa = fc->GetSimpleAttribute(subAttribute->GetCode());
			if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
			{
				auto listedValue = sa->GetListedValue(subAttribute->GetValue());
				if (listedValue)
				{
					creator->AddSimpleAttribute(featureRecord, addedCA, subAttribute->GetCode(), std::to_string(listedValue->GetCode()));
				}
			}
			else
			{
				creator->AddSimpleAttribute(featureRecord, addedCA, subAttribute->GetCode(), subAttribute->GetValue());
			}
		}
		else
		{
			//auto addedSubCA = creator->AddComplexAttribute(featureRecord, addedCA, subAttribute->GetCode());
			ConvertFromS101GML(creator, featureRecord, addedCA, (GF::ComplexAttributeType*)subAttribute);
		}
	}

	return true;
}

bool S101Cell::ConvertFeaturesFromS101GML(S10XGML* gml, S101Creator* creator)
{
	for (auto i = gml->features.begin(); i != gml->features.end(); i++)
	{
		auto feature = (*i);
		auto code = (*i)->GetCode();

		auto geometryID = feature->GetGeometryID();
		if (geometryID.empty() == false)
		{
			auto fr = new R_FeatureRecord();
			fr->SetRCID(feature->GetIDAsInteger());
			fr->SetNumericCode(m_dsgir.GetFeatureTypeCode(pugi::as_wide(code)));

			auto geometryIntID = feature->GetGeometryIDAsInt();
			if (std::string::npos != geometryID.find("mp"))
			{
				fr->SetSPAS(115, geometryIntID, 1);
			}
			else if (std::string::npos != geometryID.find("p"))
			{
				fr->SetSPAS(110, geometryIntID, 1);
			}
			else if (std::string::npos != geometryID.find("s"))
			{
				fr->SetSPAS(130, geometryIntID, 1);
			}
			else if (std::string::npos != geometryID.find("occ"))
			{
				fr->SetSPAS(125, geometryIntID, 2);
			}
			else if (std::string::npos != geometryID.find("cc"))
			{
				fr->SetSPAS(125, geometryIntID, 1);
			}
			else if (std::string::npos != geometryID.find("oc"))
			{
				fr->SetSPAS(120, geometryIntID, 2);
			}
			else if (std::string::npos != geometryID.find("c"))
			{
				fr->SetSPAS(120, geometryIntID, 1);
			}

			ConvertFromS101GML(creator, fr, feature);

			InsertFeatureRecord(fr->GetRecordName().GetName(), fr);
		}
	}

	return true;
}

bool S101Cell::ConvertInformationsFromS101GML(S10XGML* gml, S101Creator* creator)
{
	for (auto i = gml->informations.begin(); i != gml->informations.end(); i++)
	{

	}

	return true;
}

bool S101Cell::ConvertGeometriesFromS101GML(S10XGML* gml)
{
	for (auto i = gml->geometries.begin(); i != gml->geometries.end(); i++)
	{
		auto type = (*i)->GetType();
		if (type == GM::GeometryType::Point)
		{
			InsertPointRecordFromS101GML((GM::Point*)*i);
		}
		else if (type == GM::GeometryType::MultiPoint)
		{
			InsertMultiPointRecordFromS101GML((GM::MultiPoint*)*i);
		}
		else if (type == GM::GeometryType::Curve)
		{
			InsertCurveRecordFromS101GML(gml, (GM::Curve*)*i);
		}
		else if (type == GM::GeometryType::CompositeCurve)
		{
			InsertCompositeCurveRecordFromS101GML(gml, (GM::CompositeCurve*)*i);
		}
		else if (type == GM::GeometryType::Surface)
		{
			InsertSurfaceRecordFromS101GML(gml, (GM::Surface*)*i);
		}
	}

	return true;
}

bool S101Cell::InsertPointRecordFromS101GML(GM::Point* point)
{
	auto pr = new R_PointRecord();
	pr->SetRCID(point->GetIDAsInt());
	pr->SetC2IT(point->position.GetXInteger(), point->position.GetYInteger());
	InsertPointRecord(pr->GetRecordName().GetName(), pr);

	return true;
}

bool S101Cell::InsertMultiPointRecordFromS101GML(GM::MultiPoint* point)
{
	auto mr = new R_MultiPointRecord();
	mr->SetRCID(point->GetIDAsInt());

	for (auto i = point->position.begin(); i != point->position.end(); i++)
	{
		mr->InsertC3IL(i->GetXInteger(), i->GetYInteger(), (int)(i->GetZ() * GetCMFZ()));
	}

	InsertMultiPointRecord(mr->GetRecordName().GetName(), mr);

	return true;
}

bool S101Cell::InsertCurveRecordFromS101GML(S10XGML* gml, GM::Curve* curve)
{
	auto pt1 = gml->GetPoint(
		curve->segment.front().controlPoints.front().GetXInteger(),
		curve->segment.front().controlPoints.front().GetYInteger());

	auto pt2 = gml->GetPoint(
		curve->segment.front().controlPoints.back().GetXInteger(),
		curve->segment.front().controlPoints.back().GetYInteger());

	bool isClosed = curve->IsClosed();

	if (pt1 && pt2)
	{
		if (curve->segment.size() > 0)
		{
			auto cr = new R_CurveRecord();
			cr->SetRCID(curve->GetIDAsInt());

			if (isClosed)
			{
				cr->SetPTAS(pt1->GetIDAsInt());
			}
			else
			{
				cr->SetPTAS(pt1->GetIDAsInt(), pt2->GetIDAsInt());
			}

			for (int i = 1; i < curve->segment.front().controlPoints.size() - 2; i++)
			{
				cr->InsertC2IL(
					curve->segment.front().controlPoints.at(i).GetXInteger(),
					curve->segment.front().controlPoints.at(i).GetYInteger());
			}

			auto result = InsertCurveRecord(cr->GetRecordName().GetName(), cr);
			if (!result) {
				OutputDebugString(L"A\n");
				delete cr;
				cr = nullptr;
			}
		}
	}

	return true;
}

bool S101Cell::InsertCompositeCurveRecordFromS101GML(S10XGML* gml, GM::CompositeCurve* curve)
{
	auto ccr = new R_CompositeRecord();
	ccr->SetRCID(curve->GetIDAsInt());

	for (auto i = curve->component.begin(); i != curve->component.end(); i++)
	{
		//auto id = (*i)->baseCurveID;
		auto id = (*i)->GetID();

		auto curve = gml->GetOrientableCurve(id);
		if (curve) {
			if (std::string::npos != id.find("occ"))
			{
				ccr->InsertCurve(125, curve->GetIDAsInt(), 2);
			}
			else if (std::string::npos != id.find("cc"))
			{
				ccr->InsertCurve(125, curve->GetIDAsInt(), 1);
			}
			else if (std::string::npos != id.find("oc"))
			{
				ccr->InsertCurve(120, curve->GetIDAsInt(), 2);
			}
			else if (std::string::npos != id.find("c"))
			{
				ccr->InsertCurve(120, curve->GetIDAsInt(), 1);
			}
		}



		//if (std::string::npos != id.find("occ"))
		//{
		//	ccr->InsertCurve(125, (*i)->GetBaseCurveIDAsInt(), 2);
		//}
		//else if (std::string::npos != id.find("cc"))
		//{
		//	ccr->InsertCurve(125, (*i)->GetBaseCurveIDAsInt(), 1);
		//}
		//else if (std::string::npos != id.find("oc"))
		//{
		//	ccr->InsertCurve(120, (*i)->GetBaseCurveIDAsInt(), 2);
		//}
		//else if (std::string::npos != id.find("c"))
		//{
		//	ccr->InsertCurve(120, (*i)->GetBaseCurveIDAsInt(), 1);
		//}
	}

	InsertCompositeCurveRecord(ccr->GetRecordName().GetName(), ccr);

	return true;
}

bool S101Cell::InsertSurfaceRecordFromS101GML(S10XGML* gml, GM::Surface* curve)
{
	auto sr = new R_SurfaceRecord();
	sr->SetRCID(curve->GetIDAsInt());

	auto exteriorID = curve->patch.boundary.exterior->GetID();
	int exteriorIntID = curve->patch.boundary.exterior->GetIDAsInt();
	if (std::string::npos != exteriorID.find("occ"))
	{
		sr->InsertRing(125, exteriorIntID, 1, 2);
	}
	else if (std::string::npos != exteriorID.find("cc"))
	{
		sr->InsertRing(125, exteriorIntID, 1, 1);
	}
	else if (std::string::npos != exteriorID.find("oc"))
	{
		sr->InsertRing(120, exteriorIntID, 1, 2);
	}
	else if (std::string::npos != exteriorID.find("c"))
	{
		sr->InsertRing(120, exteriorIntID, 1, 1);
	}

	for (auto j = curve->patch.boundary.interior.begin();
		j != curve->patch.boundary.interior.end();
		j++)
	{
		auto interiorID = (*j)->GetID();
		int interiorIntID = (*j)->GetIDAsInt();
		if (std::string::npos != interiorID.find("occ"))
		{
			sr->InsertRing(125, interiorIntID, 2, 2);
		}
		else if (std::string::npos != interiorID.find("cc"))
		{
			sr->InsertRing(125, interiorIntID, 2, 1);
		}
		else if (std::string::npos != interiorID.find("oc"))
		{
			sr->InsertRing(120, interiorIntID, 2, 2);
		}
		else if (std::string::npos != interiorID.find("c"))
		{
			sr->InsertRing(120, interiorIntID, 2, 1);
		}
	}

	InsertSurfaceRecord(sr->GetRecordName().GetName(), sr);

	return true;
}

bool S101Cell::ATTRtoAttribute()
{
	if (false == FeatureAttrToAttribute())
	{
		return false;
	}

	if (false == InformationAttrToAttribute())
	{
		return false;
	}

	if (false == FeatureFeatureAssociationToGFM())
	{
		return false;
	}

	if (false == FeatureInformationAssociationToGFM())
	{
		return false;
	}

	if (false == InformationAssociationToGFM())
	{
		return false;
	}

	return true;
}

bool S101Cell::FeatureAttrToAttribute()
{
	auto fc = GetFC();
	int cntFeature = GetFeatureCount();

	for (int i = 0; i < cntFeature; i++) {
		std::vector<GF::ThematicAttributeType*> addedAttributes;
		auto fr = GetFeatureRecordByIndex(i);

		auto ATTRs = fr->GetAllAttributes();
		for (auto j = ATTRs.begin(); j != ATTRs.end(); j++) {
			auto ATTR = (*j);
			auto strCode = m_dsgir.GetAttributeCode(ATTR->m_natc);
			auto code = pugi::as_utf8(strCode);
			auto sa = fc->GetSimpleAttribute(std::wstring(strCode));
			if (sa)
			{
				auto value = ATTR->getValueAsString();
				CString strValue;

				strValue = LibMFCUtil::StringToWString(value).c_str();

				if (ATTR->m_paix > 0)
				{
					auto parentCA = (GF::ComplexAttributeType*)addedAttributes.at(ATTR->m_paix - 1);
					auto addedSA = parentCA->AddSubSimpleAttribute(sa->GetValueType(), code, pugi::as_utf8(std::wstring(strValue)));
					addedAttributes.push_back((GF::ThematicAttributeType*)addedSA);
				}
				else // top level
				{
					auto addedSA = fr->AddSimpleAttribute(sa->GetValueType(), code, pugi::as_utf8(std::wstring(strValue)));
					addedAttributes.push_back(addedSA);
				}
			}
			else
			{
				auto ca = fc->GetComplexAttribute(std::wstring(strCode));
				if (ca)
				{
					if (ATTR->m_paix > 0)
					{
						auto parentCA = (GF::ComplexAttributeType*)addedAttributes.at(ATTR->m_paix - 1);
						auto addedCA = parentCA->AddComplexAttribute(code);
						addedAttributes.push_back(addedCA);
					}
					else // top level
					{
						auto addedCA = fr->AddComplexAttribute(code);
						addedAttributes.push_back(addedCA);
					}
				}
				else
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool S101Cell::InformationAttrToAttribute()
{
	auto fc = GetFC();
	int cntInformation = GetInformationCount();
	for (int i = 0; i < cntInformation; i++) {
		std::vector<GF::ThematicAttributeType*> addedAttributes;
		auto ir = GetInformationRecordByIndex(i);
		auto ATTRs = ir->GetAllAttributes();
		for (auto j = ATTRs.begin(); j != ATTRs.end(); j++) {
			auto ATTR = (*j);
			auto strCode = m_dsgir.GetAttributeCode(ATTR->m_natc);
			auto code = pugi::as_utf8(strCode);
			auto sa = fc->GetSimpleAttribute(std::wstring(strCode));
			if (sa)
			{
				auto value = ATTR->getValueAsString();
				CString strValue;

				strValue = LibMFCUtil::StringToWString(value).c_str();

				if (ATTR->m_paix > 0)
				{
					auto parentCA = (GF::ComplexAttributeType*)addedAttributes.at(ATTR->m_paix - 1);
					auto addedSA = parentCA->AddSubSimpleAttribute(sa->GetValueType(), code, pugi::as_utf8(std::wstring(strValue)));
					addedAttributes.push_back((GF::ThematicAttributeType*)addedSA);
				}
				else // top level
				{
					auto addedSA = ir->AddSimpleAttribute(sa->GetValueType(), code, pugi::as_utf8(std::wstring(strValue)));
					addedAttributes.push_back(addedSA);
				}
			}
			else
			{
				auto ca = fc->GetComplexAttribute(std::wstring(strCode));
				if (ca)
				{
					auto addedCA = ir->AddComplexAttribute(code);
					addedAttributes.push_back(addedCA);
					if (ATTR->m_paix > 0)
					{
						auto parentCA = (GF::ComplexAttributeType*)addedAttributes.at(ATTR->m_paix - 1);
						parentCA->AddSubAttribute(addedCA->clone());
					}
				}
				else
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool S101Cell::FeatureFeatureAssociationToGFM()
{
	auto fc = GetFC();
	int cntFeature = GetFeatureCount();
	for (int i = 0; i < cntFeature; i++) 
	{
		auto fr = GetFeatureRecordByIndex(i);
		auto fascs = fr->GetAllFeatureAssociations();
		
		for (auto j = fascs.begin(); j != fascs.end(); j++) 
		{
			auto fasc = (*j);
			auto code = m_dsgir.GetFeatureAssociationCodeAsString(fasc->m_nfac);
			auto role = m_dsgir.GetAssociationRoleCodeAsString(fasc->m_narc);
			auto rcid = fasc->m_name.GetRCIDasString();

			fr->AddFeatureAssociation(code, role, rcid);
		}
	}

	return true;
}

bool S101Cell::FeatureInformationAssociationToGFM()
{
	auto fc = GetFC();
	int cntFeature = GetFeatureCount();
	for (int i = 0; i < cntFeature; i++)
	{
		auto fr = GetFeatureRecordByIndex(i);
		auto inass = fr->GetAllInformationAssociations();

		for (auto j = inass.begin(); j != inass.end(); j++)
		{
			auto inas = (*j);
			auto code = m_dsgir.GetInformationAssociationCodeAsString(inas->m_niac);
			auto role = m_dsgir.GetAssociationRoleCodeAsString(inas->m_narc);
			auto rcid = inas->m_name.GetRCIDasString();

			fr->AddInformationAssociation(code, role, rcid);
		}
	}

	return true;
}

bool S101Cell::InformationAssociationToGFM()
{
	auto fc = GetFC();
	int cntInformation = GetInformationCount();
	for (int i = 0; i < cntInformation; i++)
	{
		auto ir = GetInformationRecordByIndex(i);
		auto inass = ir->GetAllInformationAssociations();

		for (auto j = inass.begin(); j != inass.end(); j++)
		{
			auto inas = (*j);
			auto code = m_dsgir.GetInformationAssociationCodeAsString(inas->m_niac);
			auto role = m_dsgir.GetAssociationRoleCodeAsString(inas->m_narc);
			auto rcid = inas->m_name.GetRCIDasString();

			ir->AddInformationAssociation(code, role, rcid);
		}
	}

	return true;
}

Version S101Cell::GetVersion() const
{
   Version version;
   std::wistringstream stream(std::wstring(m_dsgir.m_dsid.m_pred));
   std::wstring segment;
   std::vector<int> parts;

   while (std::getline(stream, segment, L'.')) 
   {
       parts.push_back(std::stoi(segment));
   }

   if (parts.size() > 0)
   {
	   version.major = std::to_string(parts[0]);
   }

   if (parts.size() > 1)
   {
	   version.minor = std::to_string(parts[1]);
   }

   if (parts.size() > 2)
   {
	   version.patch = std::to_string(parts[2]);
   }

   return version;
}