#include "stdafx.h"
#include "Class_S101_Base.h"
#include <algorithm>

#define		LEADER_SIZE			24
#define		FIELD_EOF			30

CClass_S101_Base::CClass_S101_Base()
{
	m_vecInfoRecord.clear();
	m_vecPointRecord.clear();
	m_vecMultiPointRecord.clear();
	m_vecCurveRecord.clear();
	m_vecCompCurveRecord.clear();
	m_vecSurfaceRecord.clear();
	m_vecFeatureRecord.clear();
}

CClass_S101_Base::~CClass_S101_Base()
{
	m_vecInfoRecord.clear();
	m_vecPointRecord.clear();
	m_vecMultiPointRecord.clear();
	m_vecCurveRecord.clear();
	m_vecCompCurveRecord.clear();
	m_vecSurfaceRecord.clear();
	m_vecFeatureRecord.clear();
}

// A function that changes Char Array to Int.
int	CClass_S101_Base::CharDataToInt(const BYTE *pData, int nCount)
{
	int nRtn = 0;

	CString strData, strTemp;
	strData.Empty();
	for (int i = 0; i < nCount; i++)
	{
		strTemp.Format(_T("%c"), pData[i]);
		strData += strTemp;
	}

	nRtn = _tstoi(strData);

	return nRtn;
}

// A function to find Tag.
CString	CClass_S101_Base::FindTag(const BYTE *pData)
{
	CString strTag;
	strTag.Empty();

	// Record Length
	int nRecordLength = CharDataToInt(pData + 0, 5);
	if (nRecordLength == 0)
		return _T("");

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return strTag;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return strTag;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return strTag;

	// Find the count of fields.
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag

		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		if (strTag == _T("DSID") || strTag == _T("CSID") || strTag == _T("IRID") || strTag == _T("PRID") || strTag == _T("MRID") || strTag == _T("CRID") ||
			strTag == _T("CCID") || strTag == _T("SRID") || strTag == _T("DSID") || strTag == _T("FRID"))
		{
			break;
		}
	}

	return strTag;
}

// A function obtained by converting Char Data into UTF-8.
CString	CClass_S101_Base::GetUTF_8(char *pData, int &nCount)
{
	CString strRtn;

	int nIndex = 0;
	char pTemp[1024];
	ZeroMemory(pTemp, 1024);
	while (pData[nCount] != (char)(31))
	{
		pTemp[nIndex] = pData[nCount++];
		nIndex++;
	}

	strRtn = CA2W(pTemp, CP_UTF8);

	return strRtn;
}

// A function that reads S-101 data.
bool	CClass_S101_Base::LoadBaseCell(CString strFilePath)
{
	bool bRtn = false;

	HANDLE hFile, hFileMap;
	DWORD dwFileSize;

	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	fileSize = dwFileSize;
	// Use to memory as much as dwFileSize.
	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
	if (hFileMap == NULL)
	{
		CloseHandle(hFile);
		return false;
	}

	const BYTE *pData = NULL;
	pData = (BYTE *)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);

	bRtn = ParsingBaseCell(pData);

	if (pData != NULL)
	{
		UnmapViewOfFile(pData);
	}

	CloseHandle(hFileMap);
	CloseHandle(hFile);

	GenerateMapForRecord();

	return bRtn;
}

// A function that parses the read ENC file.
bool	CClass_S101_Base::ParsingBaseCell(const BYTE *pData)
{
	bool bRtn = true;

	m_vecInfoRecord.clear();
	m_vecPointRecord.clear();
	m_vecMultiPointRecord.clear();
	m_vecCurveRecord.clear();
	m_vecCompCurveRecord.clear();
	m_vecSurfaceRecord.clear();

	// Reading Header 
	int nTotalCount = 0;
	int nRecordLength = 0;
	if (Parsing_8211(pData, nRecordLength) == false)
	{
		//OutputDebugString(_T("File does not appear to have a valid ISO 8211 header!!"));
		return false;
	}
	else
	{
		CString strTag;
		nTotalCount += nRecordLength;
		strTag = FindTag(pData + nTotalCount);
		while (strTag.IsEmpty() == FALSE)
		{
			bool bFlag = false;
			if (strTag == _T("DSID"))			bFlag = Parsing_DSGI(pData + nTotalCount, nRecordLength);
			else if (strTag == _T("CSID"))		bFlag = Parsing_DCRS(pData + nTotalCount, nRecordLength);
			else if (strTag == _T("IRID"))
			{
				ST_INFO_RECORD stInfoRecord;
				if (Parsing_InfoRecord(pData + nTotalCount, stInfoRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecInfoRecord.push_back(stInfoRecord);
				}
			}
			else if (strTag == _T("PRID"))
			{
				ST_POINT_RECORD stPointRecord;
				if (Parsing_PointRecord(pData + nTotalCount, stPointRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecPointRecord.push_back(stPointRecord);
				}
			}
			else if (strTag == _T("MRID"))
			{
				ST_MULTI_POINT_RECORD stMultiPointRecord;
				if (Parsing_MultiPointRecord(pData + nTotalCount, stMultiPointRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecMultiPointRecord.push_back(stMultiPointRecord);
				}
			}
			else if (strTag == _T("CRID"))
			{
				ST_CURVE_RECORD stCurveRecord;
				if (Parsing_CurveRecord(pData + nTotalCount, stCurveRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecCurveRecord.push_back(stCurveRecord);
				}
			}
			else if (strTag == _T("CCID"))
			{
				ST_COMP_CURVE_RECORD stCompCurveRecord;
				if (Parsing_CompositeCurveRecord(pData + nTotalCount, stCompCurveRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecCompCurveRecord.push_back(stCompCurveRecord);
				}
			}
			else if (strTag == _T("SRID"))
			{
				ST_SURFACE_RECORD stSurfaceRecord;
				if (Parsing_SurfaceRecord(pData + nTotalCount, stSurfaceRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecSurfaceRecord.push_back(stSurfaceRecord);
				}
			}
			else if (strTag == _T("FRID"))
			{
				ST_FEATURE_RECORD stFeatureRecord;
				if (Parsing_FeatureRecord(pData + nTotalCount, stFeatureRecord, nRecordLength) == true)
				{
					bFlag = true;
					m_vecFeatureRecord.push_back(stFeatureRecord);
				}
			}

			if (bFlag == true)
			{
				nTotalCount += nRecordLength;
				if (nTotalCount >= fileSize)
				{
					strTag = _T("");
				}
				else
				{
					strTag = FindTag(pData + nTotalCount);
				}
			}
			else
			{
				bRtn = false;
				break;
			}
		}
	}

	return bRtn;
}

// ISO 8211 Verification Function
bool	CClass_S101_Base::Parsing_8211(const BYTE *pData, int &nRecordLength)
{
	// ISO 8211 Format Verification 
	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Interchange Level
	int nLevel = CharDataToInt(pData + 5, 1);
	if (nLevel < 1 || nLevel > 3)
	{
		return false;
	}

	// Leader identifier
	if (pData[6] != 'L')
	{
		return false;
	}

	// Version Number
	if (pData[8] != '1' && pData[8] != ' ')
	{
		return false;
	}

	return true;
}

// Data Set General Information Record
bool	CClass_S101_Base::Parsing_DSGI(const BYTE *pData, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	//Find the count of fields.
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("DSID"))		Parsing_DSID(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("DSSI"))	Parsing_DSSI(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("ATCS"))	Parsing_ATCS(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("ITCS"))	Parsing_ITCS(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("FTCS"))	Parsing_FTCS(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("IACS"))	Parsing_IACS(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("FACS"))	Parsing_FACS(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("ARCS"))	Parsing_ARCS(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("ATTR"))
		{
			ST_ATTR stATTR;
			Parsing_ATTR(pData + nTotalOffset, stATTR.vecNATC, nFieldLength);
			m_stDSGI.vecATTR.push_back(stATTR);
		}
	}

	return bRtn;
}

// Dataset Identification field
bool	CClass_S101_Base::Parsing_DSID(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;
	m_stDSGI.stDSID.nRCNM = pData[nCount++];

	m_stDSGI.stDSID.nRCID = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSID.nRCID += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSID.strENSP = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strENED = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strPRSP = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strPRED = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strPROF = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strDSNM = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strDSTL = GetUTF_8((char*)pData, nCount);
	nCount++;

	while (pData[nCount] != (char)(31))
	{
		m_stDSGI.stDSID.strDSRD += pData[nCount++];
		if (m_stDSGI.stDSID.strDSRD.GetLength() >= 8) break;
	}

	m_stDSGI.stDSID.strDSLG = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strDSAB = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDSGI.stDSID.strDSED = GetUTF_8((char*)pData, nCount);
	nCount++;

	unsigned int nDSTC = 0;
	m_stDSGI.stDSID.vecDSTC.clear();
	while (pData[nCount] != (char)(31))
	{
		nDSTC = pData[nCount++];
		m_stDSGI.stDSID.vecDSTC.push_back(nDSTC);
		if (nFieldLength - 1 <= nCount) break;
	}

	return true;
}

// Dataset Structure information field
bool	CClass_S101_Base::Parsing_DSSI(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	m_stDSGI.stDSSI.fDCOX = *(double *)&pData[nCount];
	m_stDSGI.stDSSI.fDCOY = *(double *)&pData[nCount + 8];
	m_stDSGI.stDSSI.fDCOZ = *(double *)&pData[nCount + 16];

	nCount += 24;


	m_stDSGI.stDSSI.nCMFX = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nCMFX += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nCMFY = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nCMFY += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nCMFZ = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nCMFZ += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOIR = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOIR += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOPN = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOPN += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOMN = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOMN += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOCN = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOCN += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOXN = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOXN += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOSN = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOSN += pData[nCount++] << 8 * i;
	}

	m_stDSGI.stDSSI.nNOFR = 0;
	for (int i = 0; i < 4; i++)
	{
		m_stDSGI.stDSSI.nNOFR += pData[nCount++] << 8 * i;
	}

	return true;
}

// Attribute Codes field
bool	CClass_S101_Base::Parsing_ATCS(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SUBFIELD stATCS;
		stATCS.strCode = GetUTF_8((char*)pData, nCount);
		nCount++;

		for (int i = 0; i < 2; i++)
		{
			stATCS.nNumeric += pData[nCount++] << 8 * i;
		}

		m_stDSGI.vecATCS.push_back(stATCS);
	}

	return true;
}

// Information Type Codes field
bool	CClass_S101_Base::Parsing_ITCS(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SUBFIELD stITCS;
		stITCS.strCode = GetUTF_8((char*)pData, nCount);
		nCount++;

		for (int i = 0; i < 2; i++)
		{
			stITCS.nNumeric += pData[nCount++] << 8 * i;
		}

		m_stDSGI.vecITCS.push_back(stITCS);
	}

	return true;
}

// Feature Type Codes field
bool	CClass_S101_Base::Parsing_FTCS(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SUBFIELD stFTCS;
		stFTCS.strCode = GetUTF_8((char*)pData, nCount);
		nCount++;

		for (int i = 0; i < 2; i++)
		{
			stFTCS.nNumeric += pData[nCount++] << 8 * i;
		}

		m_stDSGI.vecFTCS.push_back(stFTCS);
	}

	return true;
}

// Information Association Codes field
bool	CClass_S101_Base::Parsing_IACS(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SUBFIELD stIACS;
		stIACS.strCode = GetUTF_8((char*)pData, nCount);
		nCount++;

		for (int i = 0; i < 2; i++)
		{
			stIACS.nNumeric += pData[nCount++] << 8 * i;
		}

		m_stDSGI.vecIACS.push_back(stIACS);
	}

	return true;
}

// Feature Association Codes field
bool	CClass_S101_Base::Parsing_FACS(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SUBFIELD stFACS;
		stFACS.strCode = GetUTF_8((char*)pData, nCount);
		nCount++;

		for (int i = 0; i < 2; i++)
		{
			stFACS.nNumeric += pData[nCount++] << 8 * i;
		}

		m_stDSGI.vecFACS.push_back(stFACS);
	}

	return true;
}

// Association Role Codes field
bool	CClass_S101_Base::Parsing_ARCS(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SUBFIELD stARCS;
		stARCS.strCode = GetUTF_8((char*)pData, nCount);
		nCount++;

		for (int i = 0; i < 2; i++)
		{
			stARCS.nNumeric += pData[nCount++] << 8 * i;
		}

		m_stDSGI.vecARCS.push_back(stARCS);
	}

	return true;
}

// Attribute field(Metadata)
bool	CClass_S101_Base::Parsing_ATTR(const BYTE *pData, VEC_ATTR_NATC &vecNATC, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_ATTR_NATC stATTR;
		for (int i = 0; i < 2; i++)
		{
			stATTR.nNATC += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 2; i++)
		{
			stATTR.nATIX += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 2; i++)
		{
			stATTR.nPAIX += pData[nCount++] << 8 * i;
		}

		stATTR.nATIN = pData[nCount++];

		stATTR.strATVL = GetUTF_8((char*)pData, nCount);
		nCount++;

		vecNATC.push_back(stATTR);
	}

	return true;
}

// Dataset Coordinate Reference System Record
bool	CClass_S101_Base::Parsing_DCRS(const BYTE *pData, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	//Find the Count of fields.
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag 
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("CSID"))		Parsing_CSID(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("CRSH"))	Parsing_CRSH(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("CSAX"))	Parsing_CSAX(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("PROJ"))	Parsing_PROJ(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("GDAT"))	Parsing_GDAT(pData + nTotalOffset, nFieldLength);
		else if (strTag == _T("VDAT"))	Parsing_VDAT(pData + nTotalOffset, nFieldLength);
	}

	return bRtn;
}

// Coordinate Reference System Record Identifier field
bool	CClass_S101_Base::Parsing_CSID(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	m_stDCRS.stCSID.nRCNM = pData[nCount++];

	for (int i = 0; i < 4; i++)
	{
		m_stDCRS.stCSID.nRCID += pData[nCount++] << 8 * i;
	}

	m_stDCRS.stCSID.nNCRC = pData[nCount++];

	return true;
}

// Coordinate Reference System Header field
bool	CClass_S101_Base::Parsing_CRSH(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_CRSH stCRSH;
		stCRSH.nCRIX = pData[nCount++];
		stCRSH.nCRST = pData[nCount++];
		stCRSH.nCSTY = pData[nCount++];

		stCRSH.strCRNM = GetUTF_8((char*)pData, nCount);
		nCount++;

		stCRSH.strCRSI = GetUTF_8((char*)pData, nCount);
		nCount++;

		stCRSH.nCRSS = pData[nCount++];

		stCRSH.strSCRI = GetUTF_8((char*)pData, nCount);
		nCount++;

		m_stDCRS.vecCRSH.push_back(stCRSH);
	}

	return true;
}

// Coordinate System Axes field
bool	CClass_S101_Base::Parsing_CSAX(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	//while (nCount < nFieldLength - 1)
	{
		m_stDCRS.stCSAX.nAXTY = pData[nCount++];
		m_stDCRS.stCSAX.nAXUM = pData[nCount++];
	}

	return true;
}

// Projection field 
bool	CClass_S101_Base::Parsing_PROJ(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	m_stDCRS.stPROJ.nPROM = pData[nCount++];

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fPRP1 += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fPRP2 += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fPRP3 += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fPRP4 += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fPRP5 += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fFEAS += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stPROJ.fFNOR += pData[nCount++] << 8 * i;
	}

	return true;
}

// Geodetic Datum field
bool	CClass_S101_Base::Parsing_GDAT(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	m_stDCRS.stGDAT.strDTNM = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDCRS.stGDAT.strELNM = GetUTF_8((char*)pData, nCount);
	nCount++;

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stGDAT.fESMA += pData[nCount++] << 8 * i;
	}

	m_stDCRS.stGDAT.nESPT = pData[nCount++];

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stGDAT.fESPM += pData[nCount++] << 8 * i;
	}

	m_stDCRS.stGDAT.strCMNM = GetUTF_8((char*)pData, nCount);
	nCount++;

	for (int i = 0; i < 8; i++)
	{
		m_stDCRS.stGDAT.fCMGL += pData[nCount++] << 8 * i;
	}

	return true;
}

// Vertical Datum field
bool	CClass_S101_Base::Parsing_VDAT(const BYTE *pData, int nFieldLength)
{
	int nCount = 0;

	m_stDCRS.stVDAT.strDTNM = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDCRS.stVDAT.strDTID = GetUTF_8((char*)pData, nCount);
	nCount++;

	m_stDCRS.stVDAT.nDTSR = pData[nCount++];

	m_stDCRS.stVDAT.strSCRI = GetUTF_8((char*)pData, nCount);
	nCount++;

	return true;
}

// Information Record
bool	CClass_S101_Base::Parsing_InfoRecord(const BYTE *pData, ST_INFO_RECORD &stInfoRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	// find the count of fields
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("IRID"))		Parsing_IRID(pData + nTotalOffset, stInfoRecord, nFieldLength);
		else if (strTag == _T("ATTR"))
		{
			ST_ATTR stATTR;
			Parsing_ATTR(pData + nTotalOffset, stATTR.vecNATC, nFieldLength);
			stInfoRecord.vecATTR.push_back(stATTR);
		}
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stInfoRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// Information Type Record Identifier field
bool	CClass_S101_Base::Parsing_IRID(const BYTE *pData, ST_INFO_RECORD &stInfoRecord, int nFieldLength)
{
	int nCount = 0;

	stInfoRecord.stIRID.nRCNM = pData[nCount++];

	for (int i = 0; i < 4; i++)
	{
		stInfoRecord.stIRID.nRCID += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stInfoRecord.stIRID.nNITC += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stInfoRecord.stIRID.nRVER += pData[nCount++] << 8 * i;
	}

	stInfoRecord.stIRID.nRUIN = pData[nCount++];

	return true;
}

// Information Association field;
bool	CClass_S101_Base::Parsing_INAS(const BYTE *pData, VEC_INAS &vecINAS, int nFieldLength)
{
	int nCount = 0;

	ST_INAS stINAS;
	stINAS.nRRNM = pData[nCount++];

	for (int i = 0; i < 4; i++)
	{
		stINAS.nRRID += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stINAS.nNIAC += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stINAS.nNARC += pData[nCount++] << 8 * i;
	}

	stINAS.nIUIN = pData[nCount++];

	while (nCount < nFieldLength - 1)
	{
		ST_INAS_NATC stNATC;
		for (int i = 0; i < 2; i++)
		{
			stNATC.nNATC += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 2; i++)
		{
			stNATC.nATIX += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 2; i++)
		{
			stNATC.nPAIX += pData[nCount++] << 8 * i;
		}

		stNATC.nATIN = pData[nCount++];

		stNATC.strATVL = GetUTF_8((char*)pData, nCount);
		nCount++;

		stINAS.vecNATC.push_back(stNATC);
	}

	vecINAS.push_back(stINAS);

	return true;
}

// Point Record
bool	CClass_S101_Base::Parsing_PointRecord(const BYTE *pData, ST_POINT_RECORD &stPointRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	//find the count of fields.
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("PRID"))		Parsing_ID(pData + nTotalOffset, stPointRecord.stPRID, nFieldLength);
		else if (strTag == _T("C2IT"))	Parsing_C2IT(pData + nTotalOffset, stPointRecord.stC2IT, nFieldLength);
		else if (strTag == _T("C3IT"))	Parsing_C3IT(pData + nTotalOffset, stPointRecord.stC3IT, nFieldLength);
		else if (strTag == _T("C2FT"))	Parsing_C2FT(pData + nTotalOffset, stPointRecord.stC2FT, nFieldLength);
		else if (strTag == _T("C3FT"))	Parsing_C3FT(pData + nTotalOffset, stPointRecord.stC3FT, nFieldLength);
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stPointRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// ID Field
bool	CClass_S101_Base::Parsing_ID(const BYTE *pData, ST_ID &stID, int nFieldLength)
{
	int nCount = 0;

	stID.nRCNM = pData[nCount++];

	for (int i = 0; i < 4; i++)
	{
		stID.nRCID += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stID.nRVER += pData[nCount++] << 8 * i;
	}

	stID.nRUIN = pData[nCount++];

	return true;
}

// 2-D Integer Coordinate Tuple field
bool	CClass_S101_Base::Parsing_C2IT(const BYTE *pData, ST_C2IT &stC2IT, int nFieldLength)
{
	int nCount = 0;

	stC2IT.nYCOO = *(LONG*)&pData[nCount];
	stC2IT.nXCOO = *(LONG*)&pData[nCount + 4];

	return true;
}

// 3-D Integer Coordinate Tuple field
bool	CClass_S101_Base::Parsing_C3IT(const BYTE *pData, ST_C3IT &stC3IT, int nFieldLength)
{
	int nCount = 0;

	stC3IT.nVCID = pData[nCount++];

	stC3IT.nYCOO = *(LONG*)&pData[nCount];
	stC3IT.nXCOO = *(LONG*)&pData[nCount + 4];
	stC3IT.nZCOO = *(LONG*)&pData[nCount + 8];

	return true;
}

// 2-D Floating Coordinate Tuple field
bool	CClass_S101_Base::Parsing_C2FT(const BYTE *pData, ST_C2FT &stC2FT, int nFieldLength)
{
	int nCount = 0;

	stC2FT.fYCOO = *(double*)&pData[nCount];
	stC2FT.fXCOO = *(double*)&pData[nCount + 8];

	return true;
}

// 3-D Floating Coordinate Tuple field
bool	CClass_S101_Base::Parsing_C3FT(const BYTE *pData, ST_C3FT &stC3FT, int nFieldLength)
{
	int nCount = 0;

	stC3FT.nVCID = pData[nCount++];

	stC3FT.fYCOO = *(double*)&pData[nCount];
	stC3FT.fXCOO = *(double*)&pData[nCount + 8];
	stC3FT.fZCOO = *(double*)&pData[nCount + 16];

	return true;
}

// Multi Point Record
bool	CClass_S101_Base::Parsing_MultiPointRecord(const BYTE *pData, ST_MULTI_POINT_RECORD &stMultiPointRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	// find the count of fields
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("MRID"))		Parsing_ID(pData + nTotalOffset, stMultiPointRecord.stMRID, nFieldLength);
		else if (strTag == _T("COCC"))	Parsing_COCC(pData + nTotalOffset, stMultiPointRecord.stCOCC, nFieldLength);
		else if (strTag == _T("C2IL"))	Parsing_C2IL(pData + nTotalOffset, stMultiPointRecord.vecC2IL, nFieldLength);
		else if (strTag == _T("C3IL"))	Parsing_C3IL(pData + nTotalOffset, stMultiPointRecord.vecC3IL, nFieldLength);
		else if (strTag == _T("C2FL"))	Parsing_C2FL(pData + nTotalOffset, stMultiPointRecord.vecC2FL, nFieldLength);
		else if (strTag == _T("C3FL"))	Parsing_C3FL(pData + nTotalOffset, stMultiPointRecord.vecC3FL, nFieldLength);
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stMultiPointRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// Coordinate Control Field
bool	CClass_S101_Base::Parsing_COCC(const BYTE *pData, ST_COCC &stCOCC, int nFieldLength)
{
	int nCount = 0;

	stCOCC.nCOUI = pData[nCount++];

	for (int i = 0; i < 2; i++)
	{
		stCOCC.nCOIX += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stCOCC.nNCOR += pData[nCount++] << 8 * i;
	}

	return true;
}

// 2-D Integer Coordinate List field
bool	CClass_S101_Base::Parsing_C2IL(const BYTE *pData, VEC_C2IL &vecC2IL, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_C2IL	stC2IL;
		stC2IL.nYCOO = *(LONG*)&pData[nCount];
		stC2IL.nXCOO = *(LONG*)&pData[nCount + 4];

		vecC2IL.push_back(stC2IL);
		nCount += 8;
	}

	return true;
}

// 3-D Integer Coordinate List field
bool	CClass_S101_Base::Parsing_C3IL(const BYTE *pData, VEC_C3IL &vecC3IL, int nFieldLength)
{
	int nCount = 0;

	ST_C3IL	stC3IL;
	stC3IL.nVCID = pData[nCount++];

	while (nCount < nFieldLength - 1)
	{
		ST_C3IL_XYZ stXYZ;
		stXYZ.nYCOO = *(LONG*)&pData[nCount];
		stXYZ.nXCOO = *(LONG*)&pData[nCount + 4];
		stXYZ.nZCOO = *(LONG*)&pData[nCount + 8];

		stC3IL.vecXYZ.push_back(stXYZ);
		nCount += 12;
	}

	vecC3IL.push_back(stC3IL);

	return true;
}

// 2-D Floating Coordinate List field
bool	CClass_S101_Base::Parsing_C2FL(const BYTE *pData, VEC_C2FL &vecC2FL, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_C2FL	stC2FL;
		stC2FL.fYCOO = *(double*)&pData[nCount];
		stC2FL.fXCOO = *(double*)&pData[nCount + 8];

		vecC2FL.push_back(stC2FL);
		nCount += 16;
	}

	return true;
}

// 3-D Floating Coordinate List field
bool	CClass_S101_Base::Parsing_C3FL(const BYTE *pData, VEC_C3FL &vecC3FL, int nFieldLength)
{
	int nCount = 0;

	ST_C3FL	stC3FL;
	stC3FL.nVCID = pData[nCount++];

	while (nCount < nFieldLength - 1)
	{
		ST_C3FL_XYZ stXYZ;
		stXYZ.fYCOO = *(double*)&pData[nCount];
		stXYZ.fXCOO = *(double*)&pData[nCount + 8];
		stXYZ.fZCOO = *(double*)&pData[nCount + 16];

		stC3FL.vecXYZ.push_back(stXYZ);
		nCount += 24;
	}

	vecC3FL.push_back(stC3FL);

	return true;
}

// Curve Record
bool	CClass_S101_Base::Parsing_CurveRecord(const BYTE *pData, ST_CURVE_RECORD &stCurveRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	// find the count of fields
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("CRID"))		Parsing_ID(pData + nTotalOffset, stCurveRecord.stCRID, nFieldLength);
		else if (strTag == _T("PATS"))	Parsing_PTAS(pData + nTotalOffset, stCurveRecord.vecPTAS, nFieldLength);
		else if (strTag == _T("SECC"))	Parsing_SECC(pData + nTotalOffset, stCurveRecord.stSECC, nFieldLength);
		else if (strTag == _T("SEGH"))	Parsing_SEGH(pData + nTotalOffset, stCurveRecord.vecSEGH, nFieldLength);
		else if (strTag == _T("COCC"))	Parsing_COCC(pData + nTotalOffset, stCurveRecord.stCOCC, nFieldLength);
		else if (strTag == _T("C2IL"))	Parsing_C2IL(pData + nTotalOffset, stCurveRecord.vecC2IL, nFieldLength);
		else if (strTag == _T("C3IL"))	Parsing_C3IL(pData + nTotalOffset, stCurveRecord.vecC3IL, nFieldLength);
		else if (strTag == _T("C2FL"))	Parsing_C2FL(pData + nTotalOffset, stCurveRecord.vecC2FL, nFieldLength);
		else if (strTag == _T("C3FL"))	Parsing_C3FL(pData + nTotalOffset, stCurveRecord.vecC3FL, nFieldLength);
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stCurveRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// Point Association field
bool	CClass_S101_Base::Parsing_PTAS(const BYTE *pData, VEC_PTAS &vecPTAS, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_PTAS stPTAS;
		stPTAS.nRRNM = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stPTAS.nRRID += pData[nCount++] << 8 * i;
		}

		stPTAS.nTOPI = pData[nCount++];

		vecPTAS.push_back(stPTAS);
	}

	return true;
}

// Segment Control field
bool	CClass_S101_Base::Parsing_SECC(const BYTE *pData, ST_SECC &stSECC, int nFieldLength)
{
	int nCount = 0;

	stSECC.nSEUI = pData[nCount++];

	for (int i = 0; i < 2; i++)
	{
		stSECC.nSEIX += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stSECC.nNSEG += pData[nCount++] << 8 * i;
	}

	return true;
}

// Segment Header field
bool	CClass_S101_Base::Parsing_SEGH(const BYTE *pData, VEC_SEGH &vecSEGH, int nFieldLength)
{
	int nCount = 0;

	ST_SEGH stSEGH;
	stSEGH.nINTP = pData[nCount++];
	vecSEGH.push_back(stSEGH);

	return true;
}

// Composite Curve Record
bool	CClass_S101_Base::Parsing_CompositeCurveRecord(const BYTE *pData, ST_COMP_CURVE_RECORD &stCompCurveRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	// find the count of fields
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("CCID"))		Parsing_ID(pData + nTotalOffset, stCompCurveRecord.stCCID, nFieldLength);
		else if (strTag == _T("CCOC"))	Parsing_CCOC(pData + nTotalOffset, stCompCurveRecord.stCCOC, nFieldLength);
		else if (strTag == _T("CUCO"))
		{
			ST_CUCO stCUCO;
			Parsing_CUCO(pData + nTotalOffset, stCUCO.vecRRNM, nFieldLength);
			stCompCurveRecord.vecCUCO.push_back(stCUCO);
		}
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stCompCurveRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// Curve Component Control field
bool	CClass_S101_Base::Parsing_CCOC(const BYTE *pData, ST_CCOC &stCCOC, int nFieldLength)
{
	int nCount = 0;

	stCCOC.nCCUI = pData[nCount++];

	for (int i = 0; i < 2; i++)
	{
		stCCOC.nCCIX += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stCCOC.nNCCO += pData[nCount++] << 8 * i;
	}

	return true;
}

// Curve Component field
bool	CClass_S101_Base::Parsing_CUCO(const BYTE *pData, VEC_CUCO_RRNM &vecRRNM, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_CUCO_RRNM stRRNM;
		stRRNM.nRRNM = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nRRID += pData[nCount++] << 8 * i;
		}

		stRRNM.nORNT = pData[nCount++];

		vecRRNM.push_back(stRRNM);
	}

	return true;
}

// Surface Record
bool	CClass_S101_Base::Parsing_SurfaceRecord(const BYTE *pData, ST_SURFACE_RECORD &stSurfaceRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	// find the count of fields
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("SRID"))		Parsing_ID(pData + nTotalOffset, stSurfaceRecord.stSRID, nFieldLength);
		else if (strTag == _T("RIAS"))
		{
			ST_RIAS stRIAS;
			Parsing_RIAS(pData + nTotalOffset, stRIAS.vecRRNM, nFieldLength);
			stSurfaceRecord.vecRIAS.push_back(stRIAS);
		}
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stSurfaceRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// Ring Association field
bool	CClass_S101_Base::Parsing_RIAS(const BYTE *pData, VEC_RIAS_RRNM &vecRRNM, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_RIAS_RRNM stRRNM;
		stRRNM.nRRNM = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nRRID += pData[nCount++] << 8 * i;
		}

		stRRNM.nORNT = pData[nCount++];

		stRRNM.nUSAG = pData[nCount++];

		stRRNM.nRAUI = pData[nCount++];

		vecRRNM.push_back(stRRNM);
	}

	return true;
}

// Feature Type Record
bool	CClass_S101_Base::Parsing_FeatureRecord(const BYTE *pData, ST_FEATURE_RECORD &stFeatureRecord, int &nRecordLength)
{
	bool bRtn = true;

	// Record Length
	nRecordLength = CharDataToInt(pData + 0, 5);

	// Leader identifier
	if (pData[6] == 'R')
	{
		// means that the record is a Data Record and that a leader and directory will not be found in any of the subsequent LRs. 
		// The leader and directory of the current LR shall be applied to each of the subsequent LRs.
	}

	// Base Address of Field Area
	int nFieldAreaStart = CharDataToInt(pData + 12, 5);

	// Entry map field
	// Size of Field Length 
	int nFL = pData[20] - '0';
	if (nFL <= 0 || nFL > 9) return false;
	// Size of Field Position
	int nFP = pData[21] - '0';
	if (nFP <= 0 || nFP > 9) return false;
	// Size of Field Tag
	int nFT = pData[23] - '0';
	if (nFT <= 0 || nFT > 7) return false;

	// find the count of fields
	int nFieldEntryWidth = nFL + nFP + nFT;
	int nFieldCount = 0;
	for (int i = LEADER_SIZE; i < nRecordLength; i += nFieldEntryWidth)
	{
		if (pData[i] == FIELD_EOF)
		{
			break;
		}

		nFieldCount++;
	}

	for (int i = 0; i < nFieldCount; i++)
	{
		char szBuf[128];
		ZeroMemory(szBuf, 128);

		// Tag
		int nOffset = LEADER_SIZE + (i*nFieldEntryWidth);
		strncpy_s(szBuf, (char*)pData + nOffset, nFT);

		CString strTag = CA2WEX<>(szBuf);
		strTag.MakeUpper();

		// Field Length 
		nOffset += nFT;
		int nFieldLength = CharDataToInt(pData + nOffset, nFL);

		// Field Position 
		nOffset += nFL;
		int nFieldPos = CharDataToInt(pData + nOffset, nFP);

		int nTotalOffset = nFieldAreaStart + nFieldPos;

		if (strTag == _T("FRID"))			Parsing_FRID(pData + nTotalOffset, stFeatureRecord.stFRID, nFieldLength);
		else if (strTag == _T("FOID"))		Parsing_FOID(pData + nTotalOffset, stFeatureRecord.stFOID, nFieldLength);
		else if (strTag == _T("ATTR"))
		{
			ST_ATTR stATTR;
			Parsing_ATTR(pData + nTotalOffset, stATTR.vecNATC, nFieldLength);
			stFeatureRecord.vecATTR.push_back(stATTR);
		}
		else if (strTag == _T("SPAS"))
		{
			ST_SPAS stSPAS;
			Parsing_SPAS(pData + nTotalOffset, stSPAS.vecRRNM, nFieldLength);
			stFeatureRecord.vecSPAS.push_back(stSPAS);
		}
		else if (strTag == _T("FASC"))
		{
			ST_FASC stFASC;
			Parsing_FASC(pData + nTotalOffset, stFASC, nFieldLength);
			stFeatureRecord.vecFASC.push_back(stFASC);
		}
		else if (strTag == _T("THAS"))
		{
			ST_THAS stTHAS;
			Parsing_THAS(pData + nTotalOffset, stTHAS.vecRRNM, nFieldLength);
			stFeatureRecord.vecTHAS.push_back(stTHAS);
		}
		else if (strTag == _T("MASK"))
		{
			ST_MASK stMASK;
			Parsing_MASK(pData + nTotalOffset, stMASK.vecRRNM, nFieldLength);
			stFeatureRecord.vecMASK.push_back(stMASK);
		}
		else if (strTag == _T("INAS"))	Parsing_INAS(pData + nTotalOffset, stFeatureRecord.vecINAS, nFieldLength);
	}

	return bRtn;
}

// Feature Type Record Identifier field
bool	CClass_S101_Base::Parsing_FRID(const BYTE *pData, ST_FRID &stFRID, int nFieldLength)
{
	int nCount = 0;

	stFRID.nRCNM = pData[nCount++];

	for (int i = 0; i < 4; i++)
	{
		stFRID.nRCID += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stFRID.nNFTC += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stFRID.nRVER += pData[nCount++] << 8 * i;
	}

	stFRID.nRUIN = pData[nCount++];

	return true;
}

// Feature Object Identifier field
bool	CClass_S101_Base::Parsing_FOID(const BYTE *pData, ST_FOID &stFOID, int nFieldLength)
{
	int nCount = 0;

	for (int i = 0; i < 2; i++)
	{
		stFOID.nAGEN += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 4; i++)
	{
		stFOID.nFIDN += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stFOID.nFIDS += pData[nCount++] << 8 * i;
	}

	return true;
}

// Spatial Association field
bool	CClass_S101_Base::Parsing_SPAS(const BYTE *pData, VEC_SPAS_RRNM &vecRRNM, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_SPAS_RRNM stRRNM;

		stRRNM.nRRNM = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nRRID += pData[nCount++] << 8 * i;
		}

		stRRNM.nORNT = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nORNT += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nSMAX += pData[nCount++] << 8 * i;
		}

		stRRNM.nSAUI = pData[nCount++];

		vecRRNM.push_back(stRRNM);
	}

	return true;
}

// Feature Association field
bool	CClass_S101_Base::Parsing_FASC(const BYTE *pData, ST_FASC &stFASC, int nFieldLength)
{
	int nCount = 0;

	stFASC.nRRNM = pData[nCount++];

	for (int i = 0; i < 4; i++)
	{
		stFASC.nRRID += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stFASC.nNFAC += pData[nCount++] << 8 * i;
	}

	for (int i = 0; i < 2; i++)
	{
		stFASC.nNARC += pData[nCount++] << 8 * i;
	}

	stFASC.nFAUI = pData[nCount++];

	while (nCount < nFieldLength - 1)
	{
		ST_FASC_NATC stNATC;

		for (int i = 0; i < 2; i++)
		{
			stNATC.nNATC += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 2; i++)
		{
			stNATC.nATIX += pData[nCount++] << 8 * i;
		}

		for (int i = 0; i < 2; i++)
		{
			stNATC.nPAIX += pData[nCount++] << 8 * i;
		}

		stNATC.nATIN = pData[nCount++];

		stNATC.strATVL = GetUTF_8((char*)pData, nCount);
		nCount++;

		stFASC.vecNATC.push_back(stNATC);
	}

	return true;
}

// Theme Association field
bool	CClass_S101_Base::Parsing_THAS(const BYTE *pData, VEC_THAS_RRNM &vecRRNM, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_THAS_RRNM stRRNM;

		stRRNM.nRRNM = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nRRID += pData[nCount++] << 8 * i;
		}

		stRRNM.nTAUI = pData[nCount++];

		vecRRNM.push_back(stRRNM);
	}

	return true;
}

// Masked Spatial type field
bool	CClass_S101_Base::Parsing_MASK(const BYTE *pData, VEC_MASK_RRNM &vecRRNM, int nFieldLength)
{
	int nCount = 0;

	while (nCount < nFieldLength - 1)
	{
		ST_MASK_RRNM stRRNM;

		stRRNM.nRRNM = pData[nCount++];

		for (int i = 0; i < 4; i++)
		{
			stRRNM.nRRID += pData[nCount++] << 8 * i;
		}

		stRRNM.nMIND = pData[nCount++];

		stRRNM.nMUIN = pData[nCount++];

		vecRRNM.push_back(stRRNM);
	}

	return true;
}


ST_POINT_RECORD* CClass_S101_Base::GetPointRecord(int id)
{
	auto pointRecord = m_mapPointRecord.find(id);

	if (pointRecord != m_mapPointRecord.end())
	{
		return pointRecord->second;
	}

	return nullptr;
}


ST_MULTI_POINT_RECORD* CClass_S101_Base::GetMultiPointRecord(int id)
{
	auto multiPointRecord = m_mapMultiPointRecord.find(id);

	if (multiPointRecord != m_mapMultiPointRecord.end())
	{
		return multiPointRecord->second;
	}

	return nullptr;
}


ST_CURVE_RECORD*       CClass_S101_Base::GetCurveRecord(int id)
{
	auto curveRecord = m_mapCurveRecord.find(id);

	if (curveRecord != m_mapCurveRecord.end())
	{
		return curveRecord->second;
	}

	return nullptr;
}


ST_COMP_CURVE_RECORD*  CClass_S101_Base::GetCompositeCurveRecord(int id)
{
	auto compositeCurveRecord = m_mapCompositeCurveRecord.find(id);

	if (compositeCurveRecord != m_mapCompositeCurveRecord.end())
	{
		return compositeCurveRecord->second;
	}

	return nullptr;
}


ST_SURFACE_RECORD*     CClass_S101_Base::GetSurfaceRecord(int id)
{
	auto surfaceRecord = m_mapSurfaceRecord.find(id);

	if (surfaceRecord != m_mapSurfaceRecord.end())
	{
		return surfaceRecord->second;
	}

	return nullptr;
}


ST_FEATURE_RECORD*     CClass_S101_Base::GetFeatureRecord(int id)
{
	auto featureRecord = m_mapFeatureRecord.find(id);

	if (featureRecord != m_mapFeatureRecord.end())
	{
		return featureRecord->second;
	}

	return nullptr;
}


int CClass_S101_Base::GetGeometryTypeFromFeatureRecord(int id)
{
	ST_FEATURE_RECORD* pFeatureRecord = nullptr;
	pFeatureRecord = GetFeatureRecord(id);

	if (pFeatureRecord)
	{
		if (pFeatureRecord->vecSPAS.size() > 0)
		{
			if (pFeatureRecord->vecSPAS.front().vecRRNM.size() > 0)
			{
				return pFeatureRecord->vecSPAS.front().vecRRNM.front().nRRNM;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

bool CClass_S101_Base::CheckSPAS()
{
	for (auto i = m_vecFeatureRecord.begin(); i != m_vecFeatureRecord.end(); i++)
	{
		int cntSPAS = (int)i->vecSPAS.size();
		if (cntSPAS > 0)
		{
			int cntRRNM = (int)i->vecSPAS.begin()->vecRRNM.size();

			if (cntSPAS == 1 && cntSPAS == 1)
			{
				return true;
			}
		}
	}

	return false;
}

CString CClass_S101_Base::GetFeatureTypeCamelCase(int featureTypeCode)
{
	auto i = m_mapFeatureName.find(featureTypeCode);
	if (i == m_mapFeatureName.end())
	{
		return _T("");
	}
	return i->second;
}

CString CClass_S101_Base::GetInformationTypeCamelCase(int informationTypeCode)
{
	auto i = m_mapInformationName.find(informationTypeCode);
	if (i == m_mapInformationName.end())
	{
		return _T("");
	}
	return i->second;
}


CString CClass_S101_Base::GetAttributeCamelCase(int attributeCode)
{
	auto i = m_mapAttributeName.find(attributeCode);
	if (i == m_mapAttributeName.end())
	{
		return _T("");
	}
	return i->second;
}

void CClass_S101_Base::GetBoundingBox(float& _xmin, float& _ymin, float& _xmax, float& _ymax, int& scaleMin, int& scaleMax)
{
	int numericCodeScaleMin = GetCodeByAttributeName(_T("minimumDisplayScale"));
	int numericCodescaleMax = GetCodeByAttributeName(_T("maximumDisplayScale"));

	std::vector<int> vecScaleMin;
	std::vector<int> vecscaleMax;

	bool bFind = false;

	for (auto i = m_vecFeatureRecord.begin(); i != m_vecFeatureRecord.end(); i++)
	{
		CString featureCamelCase = GetFeatureTypeCamelCase(i->stFRID.nNFTC);
		if (!featureCamelCase.Compare(_T("DataCoverage")))
		{
			float curXmin = 0;
			float curYmin = 0;
			float curXmax = 0;
			float curYmax = 0;

			GetBoundingBox(i->stFRID.nRCID, curXmin, curYmin, curXmax, curYmax);

			if (!bFind)
			{
				_xmin = curXmin;
				_ymin = curYmin;
				_xmax = curXmax;
				_ymax = curYmax;

				bFind = true;
			}
			else
			{
				if (_xmin > curXmin)
				{
					_xmin = curXmin;
				}

				if (_xmax < curXmax)
				{
					_xmax = curXmax;
				}

				if (_ymin > curYmin)
				{
					_ymin = curYmin;
				}

				if (_ymax < curYmax)
				{
					_ymax = curYmax;
				}
			}

			for (auto j = i->vecATTR.begin(); j != i->vecATTR.end(); j++)
			{
				for (auto k = j->vecNATC.begin(); k != j->vecNATC.end(); k++)
				{
					if (numericCodeScaleMin == k->nNATC)
					{
						vecScaleMin.push_back(_ttoi(k->strATVL));
					}
					else if (numericCodescaleMax == k->nNATC)
					{
						vecscaleMax.push_back(_ttoi(k->strATVL));
					}
				}
			}
		}
	}

	std::unique(vecScaleMin.begin(), vecScaleMin.end());
	std::unique(vecscaleMax.begin(), vecscaleMax.end());

	std::sort(vecScaleMin.begin(), vecScaleMin.end());
	std::sort(vecscaleMax.begin(), vecscaleMax.end());

	if (vecScaleMin.size() > 0)
	{
		scaleMin = vecScaleMin.front();

		switch (scaleMin)
		{
		case 1:
			scaleMin = 1000;
			break;
		case 2:
			scaleMin = 2000;
			break;
		case 3:
			scaleMin = 3000;
			break;
		case 4:
			scaleMin = 4000;
			break;
		case 5:
			scaleMin = 8000;
			break;
		case 6:
			scaleMin = 12000;
			break;
		case 7:
			scaleMin = 22000;
			break;
		case 8:
			scaleMin = 45000;
			break;
		case 9:
			scaleMin = 90000;
			break;
		case 10:
			scaleMin = 180000;
			break;
		case 11:
			scaleMin = 350000;
			break;
		case 12:
			scaleMin = 700000;
			break;
		case 13:
			scaleMin = 1500000;
			break;
		case 14:
			scaleMin = 3500000;
			break;
		case 15:
			scaleMin = 10000000;
			break;
		}
	}

	if (vecscaleMax.size() > 0)
	{
		scaleMax = vecscaleMax.back();

		switch (scaleMax)
		{
		case 1:
			scaleMax = 1000;
			break;
		case 2:
			scaleMax = 2000;
			break;
		case 3:
			scaleMax = 3000;
			break;
		case 4:
			scaleMax = 4000;
			break;
		case 5:
			scaleMax = 8000;
			break;
		case 6:
			scaleMax = 12000;
			break;
		case 7:
			scaleMax = 22000;
			break;
		case 8:
			scaleMax = 45000;
			break;
		case 9:
			scaleMax = 90000;
			break;
		case 10:
			scaleMax = 180000;
			break;
		case 11:
			scaleMax = 350000;
			break;
		case 12:
			scaleMax = 700000;
			break;
		case 13:
			scaleMax = 1500000;
			break;
		case 14:
			scaleMax = 3500000;
			break;
		case 15:
			scaleMax = 10000000;
			break;
		}
	}
}


void CClass_S101_Base::GetBoundingBox(int featureID, float& xmin, float& ymin, float& xmax, float& ymax)
{
	ST_FEATURE_RECORD* pFeatureRecord = GetFeatureRecord(featureID);

	if (!pFeatureRecord)
	{
		return;
	}

	std::vector<D2D1_POINT_2F> points;
	GetPoints(pFeatureRecord, points);

	for (auto i = points.begin(); i != points.end(); i++)
	{
		if (i == points.begin())
		{
			xmin = i->x;
			ymin = i->y;
			xmax = i->x;
			ymax = i->y;
			continue;
		}

		if (xmin > i->x)
		{
			xmin = i->x;
		}
		else if (xmax < i->x)
		{
			xmax = i->x;
		}

		if (ymin > i->y)
		{
			ymin = i->y;
		}
		else if (ymax < i->y)
		{
			ymax = i->y;
		}
	}

	ymin *= -1.0;
	ymax *= -1.0;

	float temp = ymin;
	ymin = ymax;
	ymax = temp;
}


D2D1_POINT_2F CClass_S101_Base::GetPoint(ST_POINT_RECORD* pST_POINT_RECORD)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	if (pST_POINT_RECORD)
	{
		resultPoint.x = pST_POINT_RECORD->stC2IT.nXCOO / 10000000.0f;
		resultPoint.y = pST_POINT_RECORD->stC2IT.nYCOO / 10000000.0f;
	}

	return resultPoint;
}


void CClass_S101_Base::GetPoints(ST_FEATURE_RECORD *pFeatureRecord, std::vector<D2D1_POINT_2F>& points)
{
	int type = GetGeometryTypeFromFeatureRecord(pFeatureRecord->stFRID.nRCID);

	if (type == 130) // surface
	{
		ST_SURFACE_RECORD* pSurfaceRecord = GetSurfaceRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

		if (pSurfaceRecord)
		{
			GetPoints(pSurfaceRecord, points);
		}
	}
	else if (type == 120) // curve
	{
		ST_CURVE_RECORD* pCurveRecord = GetCurveRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

		if (pCurveRecord)
		{
			GetPoints(pCurveRecord, points);
		}
	}
	else if (type == 125) // comp curve
	{
		ST_COMP_CURVE_RECORD *pCompCurveRecord = GetCompositeCurveRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

		if (pCompCurveRecord)
		{
			GetPoints(pCompCurveRecord, points);
		}
	}
	else
	{
		//OutputDebugString(_T("Invalied type in GetPoints()\n"));
	}
}


void CClass_S101_Base::GetPoints(ST_CURVE_RECORD* pST_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points)
{
	D2D1_POINT_2F* pBeginningPt = nullptr;
	D2D1_POINT_2F* pEndPt = nullptr;
	D2D1_POINT_2F* pBeginningAndEndPt = nullptr;

	for (auto j = pST_CURVE_RECORD->vecPTAS.begin(); j != pST_CURVE_RECORD->vecPTAS.end(); j++)
	{
		if (j->nTOPI == 1)
		{
			double x = 0;
			double y = 0;

			ST_POINT_RECORD* pPointRecord = GetPointRecord(j->nRRID);
			if (pPointRecord)
			{
				x = pPointRecord->stC2IT.nXCOO / 10000000.0;
				y = -pPointRecord->stC2IT.nYCOO / 10000000.0;
			}

			pBeginningPt = new D2D1_POINT_2F;

			pBeginningPt->x = (FLOAT)x;
			pBeginningPt->y = (FLOAT)y;
		}
		else if (j->nTOPI == 2)
		{
			double x = 0;
			double y = 0;

			ST_POINT_RECORD* pPointRecord = GetPointRecord(j->nRRID);
			if (pPointRecord)
			{
				x = pPointRecord->stC2IT.nXCOO / 10000000.0;
				y = -pPointRecord->stC2IT.nYCOO / 10000000.0;
			}

			pEndPt = new D2D1_POINT_2F;

			pEndPt->x = (FLOAT)x;
			pEndPt->y = (FLOAT)y;
		}
		else if (j->nTOPI == 3)
		{
			double x = 0;
			double y = 0;

			ST_POINT_RECORD* pPointRecord = GetPointRecord(j->nRRID);
			if (pPointRecord)
			{
				x = pPointRecord->stC2IT.nXCOO / 10000000.0;
				y = -pPointRecord->stC2IT.nYCOO / 10000000.0;
			}

			pBeginningAndEndPt = new D2D1_POINT_2F;

			pBeginningAndEndPt->x = (FLOAT)x;
			pBeginningAndEndPt->y = (FLOAT)y;
		}
	}

	if (pBeginningPt)
	{
		points.push_back(*pBeginningPt);
	}

	if (pBeginningAndEndPt)
	{
		points.push_back(*pBeginningAndEndPt);
	}

	for (auto j = pST_CURVE_RECORD->vecC2IL.begin(); j != pST_CURVE_RECORD->vecC2IL.end(); j++)
	{
		double x = j->nXCOO / 10000000.0;
		double y = -j->nYCOO / 10000000.0;

		D2D1_POINT_2F pt;
		pt.x = (FLOAT)x;
		pt.y = (FLOAT)y;

		points.push_back(pt);
	}

	if (pEndPt)
	{
		points.push_back(*pEndPt);
	}

	if (pBeginningAndEndPt)
	{
		points.push_back(*pBeginningAndEndPt);
	}

	delete pBeginningPt;
	delete pBeginningAndEndPt;
	delete pEndPt;
}


void CClass_S101_Base::GetPoints(ST_SURFACE_RECORD* pST_SURFACE_RECORD, std::vector<D2D1_POINT_2F>& points)
{
	if (pST_SURFACE_RECORD->vecRIAS.size() > 0)
	{
		auto i = pST_SURFACE_RECORD->vecRIAS.front();
		for (auto j = i.vecRRNM.begin(); j != i.vecRRNM.end(); j++)
		{
			if (j->nUSAG == 1)
			{
				if (j->nRRNM == 120)
				{
					ST_CURVE_RECORD* pCurveRecord = GetCurveRecord(j->nRRID);
					if (pCurveRecord)
					{
						GetPoints(pCurveRecord, points);
					}
				}
				else if (j->nRRNM == 125)
				{
					ST_COMP_CURVE_RECORD* pCompCurveRecord = GetCompositeCurveRecord(j->nRRID);
					if (pCompCurveRecord)
					{
						GetPoints(pCompCurveRecord, points);
					}
				}
				else
				{
					//OutputDebugString(_T("Invalied RRNM in RIAS\n"));
				}
			}
		}
	}
}


void CClass_S101_Base::GetPoints(ST_COMP_CURVE_RECORD* pST_COMP_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points)
{
	for (auto i = pST_COMP_CURVE_RECORD->vecCUCO.begin(); i != pST_COMP_CURVE_RECORD->vecCUCO.end(); i++)
	{
		for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
		{
			if (j->nRRNM == 120)
			{
				ST_CURVE_RECORD* pCurveRecord = GetCurveRecord(j->nRRID);

				if (pCurveRecord)
				{
					std::vector<D2D1_POINT_2F> subPoints;
					GetPoints(pCurveRecord, subPoints);

					if (subPoints.size() > 0)
					{
						if (j->nORNT == 1)
						{
							for (auto k = 0; k < (int)subPoints.size(); k++)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else if (j->nORNT == 2)
						{
							for (int k = (int)subPoints.size() - 1; k >= 0; k--)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else
						{
							//OutputDebugString(_T("Unknown ORNT\n"));
						}
					}
				}
			}
			else if (j->nRRNM == 125)
			{
				ST_COMP_CURVE_RECORD* pCompositeCurveRecord = GetCompositeCurveRecord(j->nRRID);

				if (pCompositeCurveRecord)
				{
					std::vector<D2D1_POINT_2F> subPoints;
					GetPoints(pCompositeCurveRecord, subPoints);

					if (subPoints.size() > 0)
					{
						if (j->nORNT == 1)
						{
							for (auto k = 0; k < (int)subPoints.size(); k++)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else if (j->nORNT == 2)
						{
							for (int k = (int)subPoints.size() - 1; k >= 0; k--)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else
						{
							//OutputDebugString(_T("Unknown ORNT\n"));
						}
					}
				}
			}
			else
			{
				//OutputDebugString(_T("Unknown RRNM\n"));
			}
		}
	}
}


void CClass_S101_Base::GenerateMapForRecord()
{
	for (auto i = m_vecFeatureRecord.begin(); i != m_vecFeatureRecord.end(); i++)
	{
		m_mapFeatureRecord.insert({ i->stFRID.nRCID, &*i });
	}

	for (auto i = m_vecInfoRecord.begin(); i != m_vecInfoRecord.end(); i++)
	{
		m_mapInformationRecord.insert({ i->stIRID.nRCID, &*i });
	}

	for (auto i = m_vecPointRecord.begin(); i != m_vecPointRecord.end(); i++)
	{
		m_mapPointRecord.insert({ i->stPRID.nRCID, &*i });
	}

	for (auto i = m_vecMultiPointRecord.begin(); i != m_vecMultiPointRecord.end(); i++)
	{
		m_mapMultiPointRecord.insert({ i->stMRID.nRCID, &*i });
	}

	for (auto i = m_vecCurveRecord.begin(); i != m_vecCurveRecord.end(); i++)
	{
		m_mapCurveRecord.insert({ i->stCRID.nRCID, &*i });
	}

	for (auto i = m_vecCompCurveRecord.begin(); i != m_vecCompCurveRecord.end(); i++)
	{
		m_mapCompositeCurveRecord.insert({ i->stCCID.nRCID, &*i });
	}

	for (auto i = m_vecSurfaceRecord.begin(); i != m_vecSurfaceRecord.end(); i++)
	{
		m_mapSurfaceRecord.insert({ i->stSRID.nRCID, &*i });
	}

	for (auto i = m_stDSGI.vecFTCS.begin(); i != m_stDSGI.vecFTCS.end(); i++)
	{
		m_mapFeatureName.insert({ i->nNumeric, i->strCode });
	}

	for (auto i = m_stDSGI.vecITCS.begin(); i != m_stDSGI.vecITCS.end(); i++)
	{
		m_mapInformationName.insert({ i->nNumeric, i->strCode });
	}

	for (auto i = m_stDSGI.vecATCS.begin(); i != m_stDSGI.vecATCS.end(); i++)
	{
		m_mapAttributeName.insert({ i->nNumeric, i->strCode });
	}
}


void CClass_S101_Base::Output(CString& str)
{
	//OutputDebugString(str);
	str = _T("");
}


bool CClass_S101_Base::CheckBase()
{
	// Checking the number of elements in the vector and map.
	CString strOutput = _T("!!! Start Check Base !!!\n");
	Output(strOutput);

	// Check Information Record Count
	strOutput.AppendFormat(_T("Count[Information Record] : %d\n"), m_vecInfoRecord.size());
	Output(strOutput);

	// Check Point Record Count
	strOutput.AppendFormat(_T("Count[Point Record] : %d\n"), m_vecPointRecord.size());
	Output(strOutput);

	// Check Multi Point Record Count
	strOutput.AppendFormat(_T("Count[Multi Point Record] : %d\n"), m_vecMultiPointRecord.size());
	Output(strOutput);

	// Check Curve Record Count
	strOutput.AppendFormat(_T("Count[Curve Record] : %d\n"), m_vecCurveRecord.size());
	Output(strOutput);

	// Check Composite Curve Record Count
	strOutput.AppendFormat(_T("Count[Composite Curve Record] : %d\n"), m_vecCompCurveRecord.size());
	Output(strOutput);

	// Check Surface Record Count
	strOutput.AppendFormat(_T("Count[Surface Record] : %d\n"), m_vecSurfaceRecord.size());
	Output(strOutput);

	// Check Feature Record Count
	strOutput.AppendFormat(_T("Count[Feature Record] : %d\n"), m_vecFeatureRecord.size());
	Output(strOutput);

	// Check SPAS of Feature Record
	int countSPASoverONE = 0;
	int countSPASequalZERO = 0;
	int countSPAS_RRNMequalZERO = 0;
	for (auto i = m_vecFeatureRecord.begin(); i != m_vecFeatureRecord.end(); i++)
	{
		if (i->vecSPAS.size() == 0)
		{
			countSPASequalZERO++;
		}
		else if (i->vecSPAS.size() > 1)
		{
			countSPASoverONE++;
		}

		for (auto j = i->vecSPAS.begin(); j != i->vecSPAS.end(); j++)
		{
			if (j->vecRRNM.size() == 0)
			{
				countSPAS_RRNMequalZERO++;
			}
		}
	}
	strOutput.Format(_T("Check SPAS of Feature Record\n"));
	strOutput.AppendFormat(_T("SPAS > 1 : %d\n"), countSPASoverONE);
	strOutput.AppendFormat(_T("SPAS = 0 : %d\n"), countSPASequalZERO);
	strOutput.AppendFormat(_T("SPAS_RRNM = 0 : %d\n"), countSPAS_RRNMequalZERO);
	Output(strOutput);

	strOutput = _T("!!! End Check Base !!!\n");
	Output(strOutput);

	return true;
}


int CClass_S101_Base::GetCodeByAttributeName(CString attributeName)
{
	int code = -1;
	bool bFind = false;

	for (auto i = m_mapAttributeName.begin(); i != m_mapAttributeName.end(); i++)
	{
		if (!i->second.Compare(attributeName))
		{
			if (bFind == true)
			{
				//OutputDebugString(_T("Attribute name id duplicated\n"));
				return -1;
			}

			bFind = true;
			code = i->first;
		}
	}

	return code;
}


int CClass_S101_Base::GetCodeByFeatureName(CString featureName)
{
	int code = -1;
	bool bFind = false;

	for (auto i = m_mapFeatureName.begin(); i != m_mapFeatureName.end(); i++)
	{
		if (!i->second.Compare(featureName))
		{
			if (bFind == true)
			{
				//OutputDebugString(_T("Feature name id duplicated\n"));
				return -1;
			}

			bFind = true;
			code = i->first;
		}
	}

	return code;
}


int CClass_S101_Base::GetCodeByInformationName(CString informationName)
{
	int code = -1;
	bool bFind = false;

	for (auto i = m_mapInformationName.begin(); i != m_mapInformationName.end(); i++)
	{
		if (!i->second.Compare(informationName))
		{
			if (bFind == true)
			{
				//OutputDebugString(_T("Information name id duplicated\n"));
				return -1;
			}

			bFind = true;
			code = i->first;
		}
	}

	return code;
}

bool CClass_S101_Base::CheckFTCS()
{
	bool bCheck = true;

	for (auto i = m_stDSGI.vecFTCS.begin(); i != m_stDSGI.vecFTCS.end(); i++)
	{
		for (auto j = m_stDSGI.vecFTCS.begin(); j != m_stDSGI.vecFTCS.end(); j++)
		{
			if (i != j)
			{
				if (i->nNumeric == j->nNumeric)
				{
					CString strErr;
					strErr.Format(_T("[%d] Feature Numerical Code is duplicated.\n"), i->nNumeric);
					//OutputDebugString(strErr);
					bCheck = false;
				}
				else if (!i->strCode.CompareNoCase(j->strCode))
				{
					CString strErr;
					strErr.Format(_T("[%s] Feautre Code duplicates."), i->strCode);
					//OutputDebugString(strErr);
					bCheck = false;
				}
			}
		}
	}

	return bCheck;
}

void CClass_S101_Base::GetFeatureByFeatureName(CString featureName, std::vector<ST_FEATURE_RECORD*>& vecFeatrue)
{
	int featureNumericCode = GetCodeByFeatureName(featureName);
}

void CClass_S101_Base::GetFeatureByFeatureNumericCode(int numericCode, std::vector<ST_FEATURE_RECORD*>& vecFeatrue)
{
	for (auto i = vecFeatrue.begin(); i != vecFeatrue.end(); i++)
	{
	}
}