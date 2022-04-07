#include "StdAfx.h"
#include "S101Cell.h"
#include "GISLibrary.h"
#include "S101Layer.h"
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
#include "IC2D.h"
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
#include "FASC.h"
#include "MASK.h"
#include "F_SECC.h"

#include "DRDirectoryInfo.h"
#include "PCOutputSchemaManager.h"
#include "SENC_SpatialReference.h"
#include "SENC_Attribute.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_LineInstruction.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"

#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SMultiPoint.h"
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SSurface.h"
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SCurve.h"


#include "..\\LibMFCUtil\\LibMFCUtil.h"

#include <sstream> 
#include <iomanip>
#include <mmsystem.h> 
#include <unordered_map>

#pragma comment(lib, "winmm")

std::wstring get_wstring_from_coordinate_1(int value)
{
	std::wstringstream wss;
	wss.setf(std::ios::fixed);
	wss.setf(std::ios::showpoint);
	wss.precision(7);

	wss << value / 10000000.;
	return wss.str();
}

std::wstring get_wstring_from_int_1(int value)
{
	std::wstringstream wss;
	wss.precision(1);

	wss << value / 10;
	return wss.str();
}

// Key : Feature ID
// Value : Feature Element?
std::unordered_map<std::string, pugi::xml_node*> objectPugiXmlElementMap;

std::wstring get_feature_id(int id, int ENCODING = 0)
{
	wchar_t numChar[5];
	wsprintf(numChar, L"%04d", id);
	std::wstringstream wss;
	wss << L"FEATURE_ID_" << std::setw(4) << numChar;
	return wss.str();
}

std::wstring get_information_id(int id, int ENCODING = 0)
{
	wchar_t numChar[5];
	wsprintf(numChar, L"%04d", id);
	std::wstringstream wss;
	wss << L"INFO_ID_" << std::setw(4) << numChar;
	return wss.str();
}

std::string get_feature_id_string(int id)
{
	char numChar[5];
	sprintf_s(numChar, "%04d", id);
	std::stringstream ss;
	ss << "FEATURE_ID_" << std::setw(4) << numChar;
	return ss.str();
}

std::string get_information_id_string(int id)
{
	char numChar[5];
	sprintf_s(numChar, "%04d", id);
	std::stringstream ss;
	ss << "INFO_ID_" << std::setw(4) << numChar;
	return ss.str();
}

S101Cell::S101Cell() : S100SpatialObject()
{
}

S101Cell::~S101Cell()
{
	if (IsUpdate())
	{
		UpdateRemoveAll();
	}
	else //BASE file
	{
		RemoveAll();
	}
}

bool S101Cell::IsUpdate()
{
	CString filename = GetFileName();
	CString exten = LibMFCUtil::GetExtension(filename);

	if (exten.Compare(L"000") == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void S101Cell::InitDrawingInstruction()
{
	if (pcManager)
	{
		delete pcManager;
	}
	pcManager = new PCOutputSchemaManager();
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

	ClearCurveMap();

	m_vecMap.clear();

	delete gml_namespace;
	gml_namespace = nullptr;
}

void S101Cell::RemoveAll(void)
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

	ClearCurveMap();

	m_vecMap.clear();

	delete gml_namespace;
	gml_namespace = nullptr;
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
}

#pragma warning(disable:4018)
bool S101Cell::Open(CString _filepath) // Dataset start, read .000 
{
	SetFilePath(_filepath);

	USES_CONVERSION;

	RemoveAll();

	CFile file;
	if (file.Open(_filepath, CFile::modeRead))
	{
		BYTE* pBuf = nullptr;
		BYTE* sBuf = nullptr;
		BYTE* endOfBuf = nullptr;

		LONGLONG fileLength = file.GetLength();

		pBuf = new BYTE[(unsigned int)fileLength];
		sBuf = pBuf;

		file.Read(pBuf, (unsigned)fileLength);

		m_FileType = FILE_S_100_VECTOR;

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

			if (drDir.GetDirectory(0)->tag == *((unsigned int*)"DSID"))
			{
				m_dsgir.ReadRecord(&drDir, pBuf);
			}
			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CSID"))
			{
				m_dscrs.ReadRecord(&drDir, pBuf);
			}

			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"IRID"))
			{
				R_InformationRecord* r = new R_InformationRecord();
				r->ReadRecord(&drDir, pBuf);
				InsertInformationRecord(r->m_irid.m_name.GetName(), r);
			}

			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"PRID"))
			{
				R_PointRecord* r = new R_PointRecord();
				r->ReadRecord(&drDir, pBuf);
				auto names = r->m_prid.m_name.GetName();

				InsertPointRecord(r->m_prid.m_name.GetName(), r);
				m_vecMap.insert({ r->m_prid.m_name.GetName(), r });
			}
			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"MRID"))
			{
				R_MultiPointRecord* r = new R_MultiPointRecord();
				r->ReadRecord(&drDir, pBuf);

				InsertMultiPointRecord(r->m_mrid.m_name.GetName(), r);
				m_vecMap.insert({ r->m_mrid.m_name.GetName(), r });
			}
			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CRID"))   // warning and error message-oriented output
			{
				R_CurveRecord* r = new R_CurveRecord();
				r->ReadRecord(&drDir, pBuf);

				InsertCurveRecord(r->m_crid.m_name.GetName(), r);
				m_vecMap.insert({ r->m_crid.m_name.GetName(), r });
			}
			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CCID"))
			{
				R_CompositeRecord* r = new R_CompositeRecord();
				r->ReadRecord(&drDir, pBuf);

				InsertCompositeCurveRecord(r->m_ccid.m_name.GetName(), r);
				m_vecMap.insert({ r->m_ccid.m_name.GetName(), r });
			}
			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"SRID"))
			{
				R_SurfaceRecord* r = new R_SurfaceRecord();
				r->ReadRecord(&drDir, pBuf);

				InsertSurfaceRecord(r->m_srid.m_name.GetName(), r);
				m_vecMap.insert({ r->m_srid.m_name.GetName(), r });
			}
			else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"FRID")) //Feature unit processing.
			{
				R_FeatureRecord* r = new R_FeatureRecord();
				r->ReadRecord(&drDir, pBuf);

				InsertFeatureRecord(r->m_frid.m_name.GetName(), r);
			}

			pBuf = curRecordAddress + drReader.m_recordLength;
		}

		delete[] sBuf;

		MakeFullSpatialData();

		CalcMBR();
		Check();

		return true;
	}

	return false;
}

BOOL S101Cell::ReadDDR(BYTE*& buf)
{
	int size = atoi(buf, 5);
	buf -= 5;

	m_S101DDR.SetBytes(size, buf);

	buf += m_S101DDR.GetSize();

	return TRUE;
}

void S101Cell::SortByFeatureType()
{
	POSITION spasPos = NULL;

	R_FeatureRecord *fr;


	POSITION pos = m_feaMap.GetStartPosition();
	__int64 iKey;

	while (pos)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		if (fr->m_spas.size() == 0)
			continue;
	}
}

void S101Cell::GetAllFeatureDisplayOptions()
{
	R_FeatureRecord *fr;

	POSITION pos = m_feaMap.GetStartPosition();
	__int64 iKey;

	while (pos)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		GetFeatureDisplayOption(fr);
	}
}

void S101Cell::GetFeatureDisplayOption(R_FeatureRecord* pFe)
{
	//////////////////////////////////////////
	// CSymbolMap 0 : simpointsymbol.dic
	// - Symplified point symbol (ENC symbol) = point
	// CSymbolMap 1 : pacpointsymbol.dic
	// - Paper chart symbols (Paper)   = point
	// CSymbolMap 2 : linesymbol.dic
	// - Line symbole                  = line
	// CSymbolMap 3 : plainboun.dic
	// - Plain boundaries (ENC symbol) = area
	// CSymbolMap 4 : symboun.dic
	// - Symbolized boundaries (Paper) = area
	//////////////////////////////////////////

}

void S101Cell::ProcessCSProcedure()
{

}

void S101Cell::SetInstructionToFeature()
{
	POSITION spasPos = NULL;

	R_FeatureRecord *fr;

	POSITION pos = m_feaMap.GetStartPosition();
	__int64 iKey;

	while (pos)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);

#ifdef _DEBUG 
		auto iter = m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc);

		if (iter != m_dsgir.m_ftcs->m_arr.end())
		{
			CString name = iter->second->m_code;
			if (name.Find(L"Light") == 0)
			{
				name = L"";
			}
		}
#endif 

		if (fr->m_geometry)
		{
			switch (fr->m_geometry->type)
			{
			case 1:
				SetInstructionToFeature(1, fr);
				break;
			case 2:
				SetInstructionToFeature(2, fr);
				break;
			case 3:
				SetInstructionToFeature(3, fr);
				break;
			case 4:
				SetInstructionToFeature(4, fr);
				break;
			}
		}
	}
}

void S101Cell::SetInstructionToFeature(int type, R_FeatureRecord* fe)
{
	GeoPoint geoArr;
	SENC_Attribute* sencAttr = NULL;

	for (auto itorParent = fe->m_attr.begin(); itorParent != fe->m_attr.end(); itorParent++)
	{
		F_ATTR* attrParent = *itorParent;

		for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
		{
			ATTR* attr = *itor;

			auto aitor = m_dsgir.m_atcs->m_arr.find(attr->m_natc);

			if (aitor != m_dsgir.m_atcs->m_arr.end())
			{
				CodeWithNumericCode* nc = aitor->second;
				sencAttr = new SENC_Attribute();
				sencAttr->parentIndex = attr->m_paix;
				sencAttr->camelCase = nc->m_code;
				sencAttr->value = attr->m_atvl;
			}
		}
	}
}

void S101Cell::GetDDRFromFile(CString _filepath)
{
	CFile file;
	if (file.Open(_filepath, CFile::modeRead))
	{
		auto size = (int)file.GetLength();
		m_S101DDR.Allocate(size);
		file.Read(m_S101DDR.GetContent(), size);
	}
}

void S101Cell::ClearCurveMap()
{
	for (auto iter = m_curveMap.begin(); iter != m_curveMap.end(); iter++)
	{
		SCurve* c = iter->second;
		delete c;
	}

	m_curveMap.clear();
}


BOOL S101Cell::MakeFullSpatialData()
{
	ClearCurveMap();

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
	}

	return TRUE;
}

BOOL S101Cell::MakePointData(R_FeatureRecord* fe)
{
	if (fe->m_geometry)
	{
		delete fe->m_geometry;
		fe->m_geometry = nullptr;
	}

	R_PointRecord *r = nullptr;

	for (auto itorParent = fe->m_spas.begin(); itorParent != fe->m_spas.end(); itorParent++)
	{
		F_SPAS *spasParent = *itorParent;

		for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
		{
			SPAS* spas = *itor;

			__int64 iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;
			if (m_ptMap.Lookup(iKey, r))
			{
				if (r->m_c2it)
				{
					fe->m_geometry = new SPoint();
					SPoint* geo = (SPoint*)fe->m_geometry;
					GetFullSpatialData(r, geo);
					geo->m_mbr.CalcMBR(geo->m_point.x, geo->m_point.y);
				}
				else if (r->m_c3it)
				{
					GeoPointZ geoArr;

					GetFullSpatialData(r, geoArr);

					int cnt = 1;

					fe->m_geometry = new SMultiPoint();
					SMultiPoint* geo = (SMultiPoint*)fe->m_geometry;

					geo->m_numPoints = cnt;
					if (!geo->m_pPoints)
					{
						geo->m_pPoints = new std::vector<GeoPointZ>(geo->m_numPoints);
					}
					else
					{
						geo->m_pPoints->clear();
						if ((int)(*geo->m_pPoints).size() < geo->m_numPoints)
							(*geo->m_pPoints).resize(geo->m_numPoints + 1);
					}

					for (int i = 0; i < cnt; i++)
					{
						(*geo->m_pPoints)[i].SetPoint(geoArr.x, geoArr.y, geoArr.z);
					}
				}
			}
		}
	}

	return TRUE;
}
BOOL S101Cell::MakeSoundingData(R_FeatureRecord* fe)
{
	R_MultiPointRecord *r;
	__int64 iKey;

	CArray<GeoPointZ> geoArr;


	for (auto itorParent = fe->m_spas.begin(); itorParent != fe->m_spas.end(); itorParent++)
	{
		F_SPAS *spasParent = *itorParent;

		for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
		{
			SPAS* spas = *itor;

			iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;
			if (m_mpMap.Lookup(iKey, r))
			{
				GetFullSpatialData(r, geoArr);
			}
		}
	}

	int cnt = 0;
	cnt = (int)geoArr.GetCount();

	if (fe->m_geometry)
		delete fe->m_geometry;

	fe->m_geometry = new SMultiPoint();
	SMultiPoint* geo = (SMultiPoint*)fe->m_geometry;

	geo->m_numPoints = cnt;
	if (!geo->m_pPoints) geo->m_pPoints = new std::vector<GeoPointZ>(geo->m_numPoints);
	else
	{
		geo->m_pPoints->clear();
		if ((int)(*geo->m_pPoints).size() < geo->m_numPoints)
			(*geo->m_pPoints).resize(geo->m_numPoints + 1);
	}

	if (geo->m_numPoints > SGeometry::sizeOfPoint)
	{
		SGeometry::sizeOfPoint = geo->m_numPoints;
		delete SGeometry::viewPoints;
		SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
	}

	for (int i = 0; i < cnt; i++)
	{
		(*geo->m_pPoints)[i].SetPoint(geoArr[i].x, geoArr[i].y, geoArr[i].z);
		geo->m_mbr.CalcMBR(geoArr[i].x, geoArr[i].y);
	}

	geoArr.RemoveAll();
	return TRUE;
}

BOOL S101Cell::MakeLineData(R_FeatureRecord* fe)
{
	fe->m_curveList.clear();

	R_CurveRecord *cr = nullptr;

	R_CompositeRecord *ccr = nullptr;

	__int64 iKey = 0;

	if (fe->m_geometry)
	{
		delete fe->m_geometry;
		fe->m_geometry = nullptr;
	}

	for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
	{
		F_SPAS *spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			SPAS* spas = *j;
			iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;

			if (m_comMap.Lookup(iKey, ccr))
			{
				GetFullCurveData(fe, ccr, spas->m_ornt);
			}
			else if (m_curMap.Lookup(iKey, cr))
			{
				GetFullCurveData(fe, cr, spas->m_ornt);
			}
		}
	}

	SCompositeCurve *scc = new SCompositeCurve();
	fe->m_geometry = scc;

	SetSCurveList(&fe->m_curveList, &scc->m_listCurveLink);

	scc->SetMBR();

	if (gisLib == nullptr)
	{
		return false;
	}

	scc->CreateD2Geometry(gisLib->D2.pD2Factory);

	return TRUE;
}

// feature - inCurveRecordList
// 
BOOL S101Cell::SetSCurveList(std::list<OrientedCurveRecord>* inCurveRecordList, std::list<SCurveHasOrient>* outSCurveList)
{
	for (auto c = inCurveRecordList->begin(); c != inCurveRecordList->end(); c++)
	{
		OrientedCurveRecord* ocr = &(*c);

		__int64 iKey = ((__int64)ocr->m_pCurveRecord->m_crid.m_name.RCNM) << 32 | ocr->m_pCurveRecord->m_crid.m_name.RCID;
		auto curveIter = m_curveMap.find(iKey);

		bool bOrnt = ocr->m_orient == 1 ? true : false;

		if (curveIter != m_curveMap.end())
		{
			SCurveHasOrient curveHasOrient(bOrnt, curveIter->second);
			outSCurveList->push_back(curveHasOrient);
		}
		else
		{
			SCurve* pCurve = GetCurveGeometry(ocr->m_pCurveRecord);
			pCurve->m_id = iKey;
			SCurveHasOrient curveHasOrient(bOrnt, pCurve);
			outSCurveList->push_back(curveHasOrient);

			m_curveMap.insert({ iKey, pCurve });
		}
	}
	return TRUE;
}

// France
BOOL S101Cell::MakeAreaData(R_FeatureRecord* fe)
{
	fe->m_curveList.clear();

	R_SurfaceRecord *sr;
	__int64 iKey;
	CArray<GeoPoint> geoArr;
	std::vector<POINT> vecPoint;
	std::vector<int> boundaryList;

	for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
	{
		F_SPAS *spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			SPAS* spas = *j;

			iKey = spas->m_name.GetName();

			if (m_surMap.Lookup(iKey, sr))
			{
				for (auto k = sr->m_rias.begin(); k != sr->m_rias.end(); k++)
				{
					F_RIAS* riasParent = *k;

					for (auto l = riasParent->m_arr.begin(); l != riasParent->m_arr.end(); l++)
					{
						RIAS* rias = *l;

						auto iKey = rias->m_name.GetName();
						if (rias->m_name.RCNM == 120)
						{
							R_CurveRecord *cr = nullptr;
							if (m_curMap.Lookup(iKey, cr))
							{
								GetFullCurveData(fe, cr);

								GetFullSpatialData(cr, vecPoint, rias->m_ornt);
							}
						}
						else if (rias->m_name.RCNM == 125)
						{
							R_CompositeRecord *ccr = nullptr;
							if (m_comMap.Lookup(iKey, ccr))
							{
								GetFullCurveData(fe, ccr);

								GetFullSpatialData(ccr, vecPoint, rias->m_ornt);
							}
						}

						int sizet = (int)vecPoint.size();

						if (sizet == 0)
						{
							continue;
						}

						if (rias->m_usag == 1)
						{
							boundaryList.push_back(sizet);
						}
						else
						{
							boundaryList.push_back(sizet);
						}
					}
				}
			}
		}
	}

	int i = 0;

	if (fe->m_geometry)
	{
		delete fe->m_geometry;
	}

	fe->m_geometry = new SSurface(vecPoint, boundaryList);

	SSurface *geo = ((SSurface*)fe->m_geometry);

	fe->m_curveList.clear();

	R_CurveRecord *cr;
	R_CompositeRecord *ccr;

	for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
	{
		F_SPAS *spasParent = *i;

		for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
		{
			SPAS* spas = *j;
			iKey = spas->m_name.GetName();

			if (m_surMap.Lookup(iKey, sr))
			{
				GetFullCurveData(fe, sr);
			}
			else if (m_comMap.Lookup(iKey, ccr))
			{
				GetFullCurveData(fe, ccr);
			}
			else if (m_curMap.Lookup(iKey, cr))
			{
				GetFullCurveData(fe, cr);
			}
		}
	}

	SetSCurveList(&fe->m_curveList, &geo->m_listCurveLink);

	geoArr.RemoveAll();

	if (gisLib == nullptr)
	{
		return false;
	}

	geo->CreateD2Geometry(gisLib->D2.pD2Factory);

	geo->CalculateCenterPoint();

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_PointRecord *r, GeoPointZ &geo)
{
	double x = r->m_c3it->m_xcoo;
	double y = r->m_c3it->m_ycoo;
	double z = r->m_c3it->m_zcoo;

	if (m_dsgir.m_dssi.m_cmfy && m_dsgir.m_dssi.m_cmfx)
	{
		geo.SetPoint(
			x,
			y,
			z);
	}
	else
	{
		geo.SetPoint(
			x / 10000000.0,
			y / 10000000.0,
			z / 100.);
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_PointRecord *r, SPoint* point)
{
	double x = r->m_c2it->m_xcoo;
	double y = r->m_c2it->m_ycoo;

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

	projection(point->m_point.x, point->m_point.y);

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_MultiPointRecord *r, CArray<GeoPointZ> &geoArr)
{

	for (auto itor = r->m_c3il.begin(); itor != r->m_c3il.end(); itor++)
	{
		F_C3IL *c3il = *itor;
		for (auto itor = c3il->m_arr.begin(); itor != c3il->m_arr.end(); itor++)
		{
			C3IL* unitC3IL = *itor;

			double x = unitC3IL->m_xcoo;
			double y = unitC3IL->m_ycoo;

			GeoPointZ gpz;
			gpz.SetPoint(
				x / (double)m_dsgir.m_dssi.m_cmfx,
				y / (double)m_dsgir.m_dssi.m_cmfy,
				(unitC3IL->m_zcoo > 0 ? unitC3IL->m_zcoo + 0.5 : unitC3IL->m_zcoo - 0.5) / (double)m_dsgir.m_dssi.m_cmfz);
			projection(gpz.x, gpz.y);

			geoArr.Add(gpz);
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_CurveRecord *r, CArray<GeoPoint> &geoArr, int ORNT)
{
	POSITION ptasPos = NULL;
	PTAS *ptas = NULL;
	IC2D *c2di = NULL;
	R_PointRecord *spr, *epr;
	GeoPoint gp;
	__int64 iKey;

	for (auto i = r->m_ptas->m_arr.begin(); i != r->m_ptas->m_arr.end(); i++)
	{
		auto ptas = *i;

		iKey = ((__int64)ptas->m_name.RCNM) << 32 | ptas->m_name.RCID;
		if (ptas->m_topi == 1 && ORNT == 1 ||	// Beginning node , forward
			ptas->m_topi == 2 && ORNT == 2		// End node, reverse
			)
		{
			m_ptMap.Lookup(iKey, spr);
		}
		else if (ptas->m_topi == 1 && ORNT == 2 ||	// Beginning node , reverse
			ptas->m_topi == 2 && ORNT == 1		// End node, forward
			)
		{
			m_ptMap.Lookup(iKey, epr);
		}
		else if (ptas->m_topi == 3)
		{
			m_ptMap.Lookup(iKey, spr);
			epr = spr;
		}
	}


	double x = spr->m_c2it->m_xcoo;
	double y = spr->m_c2it->m_ycoo;

	gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
		y / (double)m_dsgir.m_dssi.m_cmfy);
	projection(gp.x, gp.y);
	geoArr.Add(gp);

	if (ORNT == 1)
	{
		for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
		{
			for (auto itor = (*itorParent)->m_arr.begin(); itor != (*itorParent)->m_arr.end(); itor++)
			{
				IC2D* pIC2D = *itor;

				x = pIC2D->m_xcoo;
				y = pIC2D->m_ycoo;

				gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
					y / (double)m_dsgir.m_dssi.m_cmfy);

				projection(gp.x, gp.y);
				geoArr.Add(gp);
			}
		}
	}
	else if (ORNT == 2)
	{
		for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
		{
			for (auto itor = (*itorParent)->m_arr.rbegin(); itor != (*itorParent)->m_arr.rend(); itor++)
			{
				IC2D* pIC2D = *itor;

				x = pIC2D->m_xcoo;
				y = pIC2D->m_ycoo;

				gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
					y / (double)m_dsgir.m_dssi.m_cmfy);

				projection(gp.x, gp.y);
				geoArr.Add(gp);
			}
		}
	}
	x = epr->m_c2it->m_xcoo;
	y = epr->m_c2it->m_ycoo;

	gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
		y / (double)m_dsgir.m_dssi.m_cmfy);
	projection(gp.x, gp.y);
	geoArr.Add(gp);

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_CurveRecord *r, std::vector<POINT> &geoArr, int ORNT)
{
	if (nullptr != r->m_ptas)
	{
		auto countPTAS = r->m_ptas->m_arr.size();

		if (countPTAS != 1 && countPTAS != 2)
		{
			OutputDebugString(L"Invalid count of PTAS of Curve Record\n");
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
			OutputDebugString(L"Invalied ORNT\n");
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
				OutputDebugString(L"Invalied ORNT\n");
			}
		}
		else
		{
			OutputDebugString(L"Invalied C2IL count\n");
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
			OutputDebugString(L"Invalied ORNT\n");
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_CompositeRecord *r, CArray<GeoPoint> &geoArr, int ORNT)
{
	POSITION cucoPos = NULL;
	CUCO *cuco = NULL;
	R_CurveRecord *cr = NULL;
	R_CompositeRecord *ccr = NULL;
	__int64 iKey;

	if (ORNT == 1)
	{
		for (auto itorParent = r->m_cuco.begin(); itorParent != r->m_cuco.end(); itorParent++)
		{
			F_CUCO* cucoParent = *itorParent;

			for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
			{
				cuco = *itor;
				if (cuco->m_name.RCNM == 120)
				{
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
					m_curMap.Lookup(iKey, cr);
					GetFullSpatialData(cr, geoArr, cuco->m_ornt);
				}
				else if (cuco->m_name.RCNM == 125)
				{
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
					m_comMap.Lookup(iKey, ccr);
					GetFullSpatialData(ccr, geoArr, cuco->m_ornt);
				}
			}
		}
	}
	else if (ORNT == 2)
	{
		for (auto itorParent = r->m_cuco.begin(); itorParent != r->m_cuco.end(); itorParent++)
		{
			F_CUCO* cucoParent = *itorParent;

			for (auto itor = cucoParent->m_arr.rbegin(); itor != cucoParent->m_arr.rend(); itor++)
			{
				cuco = *itor;
				iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;

				unsigned ornt = 1;
				if (cuco->m_ornt == 2)
					ornt = 1;
				else
					ornt = 2;

				if (cuco->m_name.RCNM == 120)
				{
					m_curMap.Lookup(iKey, cr);
					GetFullSpatialData(cr, geoArr, ornt);
				}
				else if (cuco->m_name.RCNM == 125)
				{
					m_comMap.Lookup(iKey, ccr);
					GetFullSpatialData(ccr, geoArr, ornt);
				}
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_CompositeRecord *r, std::vector<POINT> &geoArr, int ORNT)
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
				OutputDebugString(L"Invalid RCNM in CUCO\n");
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullSpatialData(R_SurfaceRecord *r, CArray<GeoPoint> &geoArr)
{
	RIAS *rias = NULL;
	R_CurveRecord *cr = NULL;
	R_CompositeRecord *ccr = NULL;
	__int64 iKey;

	///////////////////////////////
	// for blank interior area
	int sp = 0;
	CList<int> boundaryList;
	BOOL isExtrior = TRUE;
	///////////////////////////////

	for (auto itorParent = r->m_rias.begin(); itorParent != r->m_rias.end(); itorParent++)
	{
		F_RIAS* riasParent = *itorParent;

		for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
		{
			RIAS* rias = *itor;
			///////////////////////////////
			// for blank interior area
			if (sp == -1)
			{
				sp = (int)geoArr.GetCount();
			}
			//////////////////////////////

			iKey = ((__int64)rias->m_name.RCNM) << 32 | rias->m_name.RCID;
			if (rias->m_name.RCNM == 120)
			{
				m_curMap.Lookup(iKey, cr);
				GetFullSpatialData(cr, geoArr, rias->m_ornt);
			}
			else if (rias->m_name.RCNM == 125)
			{
				m_comMap.Lookup(iKey, ccr);
				GetFullSpatialData(ccr, geoArr, rias->m_ornt);
			}
			///////////////////////////////
			// for blank interior area
			int sizet = (int)geoArr.GetCount() - 1;
			if (geoArr[sp].x == geoArr[sizet].x &&
				geoArr[sp].y == geoArr[sizet].y)
			{
				if (isExtrior)
				{
					sp = -1;
					isExtrior = false;
				}
				else
				{
					sp = -1;
					boundaryList.AddTail(sizet + 1);
				}
			}
			//////////////////////////////
			//////////////////////////////////////////////////////////
		}
	}
	///////////////////////////////
	// for blank interior area
	int arrIndex = 0;
	int i = 0;
	POSITION boundaryPos = boundaryList.GetHeadPosition();
	int index;
	int count = 0;
	while (boundaryPos != NULL)
	{
		index = boundaryList.GetNext(boundaryPos) + count;
		GeoPoint p(geoArr[0].x, geoArr[0].y);
		geoArr.InsertAt(index, p);
		count++;
	}
	return TRUE;
}


SCurve* S101Cell::GetCurveGeometry(R_CurveRecord *r)
{
	POSITION ptasPos = NULL;
	PTAS *ptas = NULL;
	IC2D *c2di = NULL;
	R_PointRecord *spr, *epr;
	GeoPoint gp;
	__int64 iKey;
	int coordinateIndex = 0;

	for (auto i = r->m_ptas->m_arr.begin(); i != r->m_ptas->m_arr.end(); i++)
	{
		auto ptas = *i;

		iKey = ((__int64)ptas->m_name.RCNM) << 32 | ptas->m_name.RCID;
		if (ptas->m_topi == 1)
		{
			m_ptMap.Lookup(iKey, spr);
		}
		else if (ptas->m_topi == 2)
		{
			m_ptMap.Lookup(iKey, epr);
		}
		else if (ptas->m_topi == 3)
		{
			m_ptMap.Lookup(iKey, spr);
			epr = spr;
		}
	}

	SCurve* retCurve = new SCurve();

	int totalCoordinateCount = 2; // start / end 
	for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
	{
		totalCoordinateCount += (int)(*itorParent)->m_arr.size();
	}

	if (totalCoordinateCount > SGeometry::sizeOfPoint)
	{
		SGeometry::sizeOfPoint = totalCoordinateCount;
		delete SGeometry::viewPoints;
		SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
	}

	retCurve->m_numPoints = totalCoordinateCount;
	retCurve->m_pPoints = new GeoPoint[totalCoordinateCount];

	double x = spr->m_c2it->m_xcoo;
	double y = spr->m_c2it->m_ycoo;

	gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
		y / (double)m_dsgir.m_dssi.m_cmfy);
	projection(gp.x, gp.y);

	retCurve->m_pPoints[coordinateIndex++].SetPoint(gp.x, gp.y);
	retCurve->m_mbr.CalcMBR(gp.x, gp.y);

	for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
	{
		for (auto itor = (*itorParent)->m_arr.begin(); itor != (*itorParent)->m_arr.end(); itor++)
		{
			IC2D* pIC2D = *itor;

			x = pIC2D->m_xcoo;
			y = pIC2D->m_ycoo;

			gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
				y / (double)m_dsgir.m_dssi.m_cmfy);

			projection(gp.x, gp.y);

			retCurve->m_pPoints[coordinateIndex++].SetPoint(gp.x, gp.y);
			retCurve->m_mbr.CalcMBR(gp.x, gp.y);
		}
	}

	x = epr->m_c2it->m_xcoo;
	y = epr->m_c2it->m_ycoo;

	gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
		y / (double)m_dsgir.m_dssi.m_cmfy);
	projection(gp.x, gp.y);

	retCurve->m_pPoints[coordinateIndex].SetPoint(gp.x, gp.y);
	retCurve->m_mbr.CalcMBR(gp.x, gp.y);

	return retCurve;
}

BOOL S101Cell::GetFullCurveData(R_FeatureRecord* fe, R_CurveRecord *r, int ornt)
{
	OrientedCurveRecord ocr;
	ocr.m_pCurveRecord = r;
	ocr.m_orient = ornt;
	fe->m_curveList.push_back(ocr);
	return TRUE;
}

BOOL S101Cell::GetFullCurveData(R_FeatureRecord* fe, R_CompositeRecord *r, int ornt)
{
	R_CurveRecord *cr = NULL;
	R_CompositeRecord *ccr = NULL;
	__int64 iKey;

	// forward
	if (1 == ornt)
	{
		for (auto i = r->m_cuco.begin(); i != r->m_cuco.end(); i++)
		{
			F_CUCO* cucoParent = *i;

			for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
			{
				auto cuco = *itor;

				if (cuco->m_ornt == 2)
					ornt = (ornt == 2) ? 1 : 2;

				if (cuco->m_name.RCNM == 120)
				{
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
					m_curMap.Lookup(iKey, cr);

					GetFullCurveData(fe, cr, cuco->m_ornt);
				}
				else if (cuco->m_name.RCNM == 125)
				{
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
					m_comMap.Lookup(iKey, ccr);
					GetFullCurveData(fe, ccr, cuco->m_ornt);
				}
			}
		}
	}
	else if (2 == ornt)
	{
		for (auto i = r->m_cuco.rbegin(); i != r->m_cuco.rend(); i++)
		{
			F_CUCO* cucoParent = *i;

			for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
			{
				auto cuco = *itor;

				if (cuco->m_ornt == 2)
					ornt = (ornt == 2) ? 1 : 2;

				if (cuco->m_name.RCNM == 120)
				{
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
					m_curMap.Lookup(iKey, cr);

					GetFullCurveData(fe, cr, cuco->m_ornt);
				}
				else if (cuco->m_name.RCNM == 125)
				{
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
					m_comMap.Lookup(iKey, ccr);
					GetFullCurveData(fe, ccr, cuco->m_ornt);
				}
			}
		}
	}

	return TRUE;
}

BOOL S101Cell::GetFullCurveData(R_FeatureRecord* fe, R_SurfaceRecord *r, int ornt)
{
	R_CurveRecord *cr = NULL;
	R_CompositeRecord *ccr = NULL;
	__int64 iKey;

	for (auto itorParent = r->m_rias.begin(); itorParent != r->m_rias.end(); itorParent++)
	{
		F_RIAS* riasParent = *itorParent;

		for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
		{
			RIAS* rias = *itor;

			if (rias->m_ornt == 2)
				ornt = (ornt == 2) ? 1 : 2;

			iKey = ((__int64)rias->m_name.RCNM) << 32 | rias->m_name.RCID;
			if (rias->m_name.RCNM == 120)
			{
				m_curMap.Lookup(iKey, cr);
				GetFullCurveData(fe, cr, ornt);
			}
			else if (rias->m_name.RCNM == 125)
			{
				m_comMap.Lookup(iKey, ccr);
				GetFullCurveData(fe, ccr, ornt);
			}
		}
	}
	return TRUE;
}

BOOL S101Cell::GetFullMaskData(R_FeatureRecord* fe)
{
	std::list<SCurveHasOrient>* listCurveLink = nullptr;
	if (fe->m_geometry->type == 2) // Line
	{
		SCompositeCurve* geo = (SCompositeCurve*)fe->m_geometry;
		listCurveLink = &geo->m_listCurveLink;
	}
	else if (fe->m_geometry->type == 3) // Line
	{
		SSurface* geo = (SSurface*)fe->m_geometry;
		listCurveLink = &geo->m_listCurveLink;
	}
	else
	{
		return FALSE;
	}

	for (auto iter = listCurveLink->begin(); iter != listCurveLink->end(); iter++)
	{
		SCurve* c = (*iter).GetCurve();

		for (auto itorParent = fe->m_mask.begin(); itorParent != fe->m_mask.end(); itorParent++)
		{
			F_MASK *maskParent = *itorParent;
			auto mi = maskParent->m_arr.find(c->m_id);
			if (mi == maskParent->m_arr.end())
			{
				(*iter).SetMasking(true);
			}
		}
	}

	return TRUE;
}


void S101Cell::Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler)
{
	auto rt = D2->pRT;
	rt->FillRectangle(D2D1::RectF(0, 0, 100, 100), D2->pBrush);
}


void S101Cell::Draw(HDC &hDC, Scaler *scaler, double offset)
{
}

void S101Cell::Draw(HDC &hDC, Scaler *scaler, int priority, int instructionType, double offset)
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

	R_FeatureRecord *fr = nullptr;

	__int64 iKey;
	POSITION pos = m_feaMap.GetStartPosition();
	while (pos != NULL)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);
		if (fr->m_geometry)
		{
			if (fr->m_geometry->type == 3)
			{
				SSurface *pSr = (SSurface *)fr->m_geometry;
				pMBR->ReMBR(pSr->m_mbr);
			}
			else if (fr->m_geometry->type == 2)
			{
				auto geo = (SCompositeCurve*)fr->m_geometry;
				pMBR->ReMBR(geo->m_mbr);
			}
			else if (fr->m_geometry->type == 1)
			{
				SPoint *geo = (SPoint *)fr->m_geometry;
				pMBR->ReMBR(geo->m_mbr);
			}
		}
	}

	return *pMBR;
}

// [ Text Placement ]
void S101Cell::CheckHasTextPlacement()
{
	POSITION pos = NULL;
	__int64 iKey;

	R_FeatureRecord *fr;
	pos = m_feaMap.GetStartPosition();


	int numeric_number_of_text_placement = 0;
	auto ii = m_dsgir.m_ftcs->m_arrFindForCode.find(L"TextPlacement");
	if (ii != m_dsgir.m_ftcs->m_arrFindForCode.end())
	{
		numeric_number_of_text_placement = ii->second->m_nmcd;
	}

	while (pos != NULL)
	{
		m_feaMap.GetNextAssoc(pos, iKey, fr);

		if (fr->m_frid.m_nftc == numeric_number_of_text_placement)
		{
			int numeric_number_of_bearing = 0;
			auto ii = m_dsgir.m_atcs->m_arrFindForCode.find(L"flipBearing");
			if (ii != m_dsgir.m_atcs->m_arrFindForCode.end())
			{
				numeric_number_of_bearing = ii->second->m_nmcd;
			}

			for (auto itorParent = fr->m_attr.begin(); itorParent != fr->m_attr.end(); itorParent++)
			{
				F_ATTR* attr = *itorParent;
				for (auto aitor = attr->m_arr.begin(); aitor != attr->m_arr.end(); aitor++)
				{
					ATTR* attr = *aitor;

					if (attr->m_natc == numeric_number_of_bearing)
					{
						fr->m_textBearing = new float((float)_wtof(attr->m_atvl));
					}
				}
			}
			continue;
		}

		if (fr->m_fasc.size() == 0)
		{
			continue;
		}

		for (auto iterFasc = fr->m_fasc.begin(); iterFasc != fr->m_fasc.end(); iterFasc++)
		{
			F_FASC* f_fasc = *iterFasc;
			__int64 ikey2 = f_fasc->m_name.GetName();
			auto feturemap = m_feaMap.PLookup(ikey2);
			if (feturemap == nullptr)
			{
				return;
			}
			R_FeatureRecord *rfr = feturemap->value;

			if (rfr->m_frid.m_nftc == numeric_number_of_text_placement)
			{
				fr->m_hasTextPlacement = true;

				rfr->m_scaleMin = fr->m_scaleMin;
				rfr->m_scaleMax = fr->m_scaleMax;
				break;
			}
		}
	}
}

void S101Cell::ProcessSpatialReference()
{
	if (pcManager == nullptr)
	{
		return;
	}

	if (pcManager->displayListSENC == nullptr)
	{
		return;
	}

	for (auto itor = pcManager->displayListSENC->displayInstructions.begin();
		itor != pcManager->displayListSENC->displayInstructions.end();
		itor++)
	{
		SENC_Instruction* it = itor->second;

		if (it->type == 2)
		{
			SENC_LineInstruction* inst = (SENC_LineInstruction*)it;

			if (inst->spatialReference.size() > 0)
			{
				R_FeatureRecord* fr = inst->fr;

				for (auto it = inst->spatialReference.begin(); it != inst->spatialReference.end(); it++)
				{
					unsigned ref = (*it)->reference;
				}
			}
		}
	}
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

void S101Cell::InsertInformationRecord(__int64 key, R_InformationRecord* record)
{
	m_infMap.SetAt(key, record);
	vecInformation.push_back(record);
}

void S101Cell::RemoveInformationRecord(__int64 key, R_InformationRecord* record)
{
	m_infMap.RemoveKey(key);
}
R_InformationRecord* S101Cell::GetInformationRecord(__int64 key)
{
	R_InformationRecord* item = nullptr;
	if (TRUE == m_infMap.Lookup(key, item))
	{
		return item;
	}

	return nullptr;
}

POSITION S101Cell::GetInfoStartPosition()
{
	return m_infMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_InformationRecord*& value)
{
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

std::vector<R_InformationRecord*>* S101Cell::GetVecInformation()
{
	return &vecInformation;
}

void S101Cell::InsertPointRecord(__int64 key, R_PointRecord* record)
{
	m_ptMap.SetAt(key, record);
	vecPoint.push_back(record);
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

std::vector<R_PointRecord*>* S101Cell::GetVecPoint()
{
	return &vecPoint;
}

void S101Cell::InsertMultiPointRecord(__int64 key, R_MultiPointRecord* record)
{
	m_mpMap.SetAt(key, record);
	vecMultiPoint.push_back(record);
}

void S101Cell::RemoveMultiPointRecord(__int64 key, R_MultiPointRecord* record)
{
	m_mpMap.RemoveKey(key);
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

std::vector<R_MultiPointRecord*>* S101Cell::GetVecMultiPoint()
{
	return &vecMultiPoint;
}

void S101Cell::InsertCurveRecord(__int64 key, R_CurveRecord* record)
{
	m_curMap.SetAt(key, record);
	vecCurve.push_back(record);
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

std::vector<R_CurveRecord*>* S101Cell::GetVecCurve()
{
	return &vecCurve;
}

void S101Cell::InsertCompositeCurveRecord(__int64 key, R_CompositeRecord* record)
{
	m_comMap.SetAt(key, record);
	vecComposite.push_back(record);
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

std::vector<R_CompositeRecord*>* S101Cell::GetVecComposite()
{
	return &vecComposite;
}

void S101Cell::InsertSurfaceRecord(__int64 key, R_SurfaceRecord* record)
{
	m_surMap.SetAt(key, record);
	vecSurface.push_back(record);
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

std::vector<R_SurfaceRecord*>* S101Cell::GetVecSurface()
{
	return &vecSurface;
}

void S101Cell::InsertFeatureRecord(__int64 key, R_FeatureRecord* record)
{
	m_feaMap.SetAt(key, record);
	vecFeature.push_back(record);
}

void S101Cell::RemoveFeatureRecord(__int64 key, R_FeatureRecord* record)
{
	m_feaMap.RemoveKey(key);
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

POSITION S101Cell::GetFeatureStartPosition()
{
	return m_feaMap.GetStartPosition();
}

void S101Cell::GetNextAssoc(POSITION& index, long long& key, R_FeatureRecord*& value)
{
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

std::vector<R_FeatureRecord*>* S101Cell::GetVecFeature()
{
	return &vecFeature;
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
		OutputDebugString(L"error - Information Record count\n");
		return false;
	}

	if (GetCount_PointRecord() != GetMetaCount_PointRecord())
	{
		OutputDebugString(L"error - Point Record count\n");
		return false;
	}

	if (GetCount_MultiPointRecord() != GetMetaCount_MultiPointRecord())
	{
		OutputDebugString(L"error - Multi Point Record count\n");
		return false;
	}

	if (GetCount_CurveRecord() != GetMetaCount_CurveRecord())
	{
		OutputDebugString(L"error - Curve Record count\n");
		return false;
	}

	if (GetCount_CompositeCurveRecord() != GetMetaCount_CompositeCurveRecord())
	{
		OutputDebugString(L"error - Composite Curve Record count\n");
		return false;
	}

	if (GetCount_SurfaceRecord() != GetMetaCount_SurfaceRecord())
	{
		OutputDebugString(L"error - Surface Record count\n");
		return false;
	}

	if (GetCount_FeatureTypeRecord() != GetMetaCount_FeatureTypeRecord())
	{
		OutputDebugString(L"error - Feature Type Record count\n");
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
	_int64 infokey;
	POSITION pos = cell->m_infMap.GetStartPosition(); //change Update Attri to NC
	while (pos != nullptr)
	{
		R_InformationRecord* ir = new R_InformationRecord(); //UPdate value
		cell->m_infMap.GetNextAssoc(pos, infokey, ir);

		//change itcs
		auto updateCode = itcs->GetFeatureCode(ir->m_irid.m_nitc);
		int NewCode = m_dsgir.m_itcs->GetCode(updateCode);
		ir->m_irid.m_nitc = NewCode;

		UpdateAttrField(ir->m_attr, atcs);      //attr
		UpdateInasField(ir->m_inas, iacs);		//INAS

	}

	//point map
	_int64 posKey;
	pos = cell->m_ptMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_PointRecord* pt = new R_PointRecord();
		cell->m_ptMap.GetNextAssoc(pos, posKey, pt);
		UpdateInasField(pt->m_inas, iacs);

	}

	//multi point
	_int64 muitKey;
	pos = cell->m_mpMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_MultiPointRecord* mt = new R_MultiPointRecord();
		cell->m_mpMap.GetNextAssoc(pos, muitKey, mt);
		UpdateInasField(mt->m_inas, iacs);
	}

	_int64 curKey;
	pos = cell->m_curMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_CurveRecord* cur = new R_CurveRecord();
		cell->m_curMap.GetNextAssoc(pos, curKey, cur);
		UpdateInasField(cur->m_inas, iacs);
		//PATS
	}


	_int64 conpoKey;
	pos = cell->m_comMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_CompositeRecord* com = new R_CompositeRecord();
		cell->m_comMap.GetNextAssoc(pos, conpoKey, com);
		UpdateInasField(com->m_inas, iacs);
	}


	_int64 surKey;
	pos = cell->m_surMap.GetStartPosition();
	while (pos != nullptr)
	{
		R_SurfaceRecord* sur = new R_SurfaceRecord();
		cell->m_surMap.GetNextAssoc(pos, surKey, sur);
		UpdateInasField(sur->m_inas, iacs);
	}

	//change FeatureType Record ATTR
	_int64 feakey;
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
		POSITION pos = f_inas->m_arr.GetHeadPosition();
		while (pos)
		{
			ATTR* arr = f_inas->m_arr.GetNext(pos);
			CString updateCode = iacs->GetFeatureCode(arr->m_natc);
			int NewCode = m_dsgir.m_iacs->GetCode(updateCode);

			arr->m_natc = NewCode;
		}
	}
	return true;
}

bool S101Cell::UpdateInfMapRecord(S101Cell* cell)
{
	_int64 key;
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
				RemoveInformationRecord(UpdateName, values);
			}
			else if (idvalue == 3) //Modify
			{
				//IRID
				values->m_irid.m_name.RCNM = ir->m_irid.m_name.RCNM;
				values->m_irid.m_name.RCID = ir->m_irid.m_name.RCID;
				values->m_irid.m_nitc = ir->m_irid.m_nitc; //I'm going to change the numericcode.
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

			//ATTR
			POSITION pos = f_inas->m_arr.GetHeadPosition();
			while (pos)
			{
				ATTR* arr = f_inas->m_arr.GetNext(pos);
				auto UpdateCode = arr->m_atin;

				if (1 == UpdateCode)
				{
					BaseAttr->m_arr.AddTail(arr);
				}

				else if (2 == UpdateCode) //delte
				{
					POSITION BasePos = BaseAttr->m_arr.GetHeadPosition();
					while (BasePos)
					{
						ATTR* basear = BaseAttr->m_arr.GetNext(BasePos);
						if (arr->m_natc == basear->m_natc)
						{
							BaseAttr->m_arr.RemoveAt(BasePos);
						}
					}
				}

				else if (3 == UpdateCode)//modify
				{
					POSITION BasePos = BaseAttr->m_arr.GetHeadPosition();
					while (BasePos)
					{
						ATTR* basear = BaseAttr->m_arr.GetNext(BasePos);
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
	_int64 key;
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
	_int64 key;
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
				RemoveMultiPointRecord(UpdateName, values);
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
	_int64 key;
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
				if (cur->m_segh.size() > 0)
				{
					value->m_segh.begin() = cur->m_segh.begin();
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
	_int64 key;
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
	_int64 key;
	POSITION pos = cell->m_surMap.GetStartPosition();
	while (pos != NULL)
	{
		R_SurfaceRecord * sur = new R_SurfaceRecord();
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

	_int64 key;
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
				RemoveFeatureRecord(UpdateName, values);
			}
			else if (mission == 3) //change
			{
				//FRID
				values->m_frid.m_name.RCNM = fe->m_frid.m_name.RCNM;
				values->m_frid.m_name.RCID = fe->m_frid.m_name.RCID;
				values->m_frid.m_nftc = fe->m_frid.m_nftc;
				values->m_frid.m_rver = fe->m_frid.m_rver;

				//FOID
				values->m_foid.m_objName.m_agen = fe->m_foid.m_objName.m_agen;
				values->m_foid.m_objName.m_fidn = fe->m_foid.m_objName.m_fidn;
				values->m_foid.m_objName.m_fids = fe->m_foid.m_objName.m_fids;

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
						for (FASC* fasc : f_fasc->m_arr)
						{
							int attriMission = fasc->m_atin;
							auto it = std::find(values->m_fasc.begin(), values->m_fasc.end(), f_fasc);


							if (1 == attriMission) //insert
							{
								(*it)->m_arr.push_back(fasc);
							}

							else if (2 == attriMission) //delete
							{
								auto itor = std::find((*it)->m_arr.begin(), (*it)->m_arr.end(), fasc);
								(*it)->m_arr.erase(itor);
							}

							else if (3 == attriMission) //modify
							{
								auto itor = std::find((*it)->m_arr.begin(), (*it)->m_arr.end(), fasc);
								*itor = fasc;
							}
						}
					}
				}

				int i = 0;
				for (auto f_mask : fe->m_mask)
				{
					for (auto mask : f_mask->m_arr)
					{
						auto Key = (__int64)mask.second->m_name.GetName();
						auto mission = mask.second->m_muin;

						auto it = values->m_mask.begin();
						std::advance(it, i);

						if (1 == mission) //insert
						{
							(*it)->m_arr.insert({ Key, mask.second });

						}
						else //delete
						{
							(*it)->m_arr.erase(Key);
						}
					}
				}
			}
		}
	}
	return true;
}