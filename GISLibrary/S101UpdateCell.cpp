#include "stdafx.h"
#include "S101UpdateCell.h"
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
//#include <unordered_map>
//#include <vector>
#include <string>
#include <set>
//#include <cstdint>


S101UpdateCell::S101UpdateCell(D2D1Resources* d2d1)
	: S101Cell(d2d1)
{
}

S101UpdateCell::S101UpdateCell(FeatureCatalogue* fc, D2D1Resources* d2d1)
	: S101Cell(fc, d2d1)
{
}

S101UpdateCell::~S101UpdateCell()
{
	RemoveAll();
}


void S101UpdateCell::ClearAll(void)
{
	// 업데이트 관련 정리
	m_updateItems.clear();
	//m_rcnmIndex.clear();
}

void S101UpdateCell::RemoveAll(void)
{
	// 업데이트 항목 정리
	ClearAll();

	// 부모 클래스의 RemoveAll 호출
	S101Cell::RemoveAll();
}

// 업데이트 항목 생성 (로드된 모든 데이터를 업데이트 항목으로)
void S101UpdateCell::CreateUpdateItems()
{
	m_updateItems.clear();
	//m_rcnmIndex.clear();

	// Information Records
	for (int i = 0; i < GetCount_InformationRecord(); i++) {
		R_InformationRecord* record = GetInformationRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_irid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin, GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}

			// Point Records
	for (int i = 0; i < GetCount_PointRecord(); i++) {
		R_PointRecord* record = GetPointRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_prid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin, GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}

	// MultiPoint Records
	for (int i = 0; i < GetCount_MultiPointRecord(); i++) {
		R_MultiPointRecord* record = GetMultiPointRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_mrid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin,GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}

	// Curve Records
	for (int i = 0; i < GetCount_CurveRecord(); i++) {
		R_CurveRecord* record = GetCurveRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_crid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin, GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}

	// Composite Records
	for (int i = 0; i < GetCount_CompositeCurveRecord(); i++) {
		R_CompositeRecord* record = GetCompositeCurveRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_ccid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin, GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}

	// Surface Records
	for (int i = 0; i < GetCount_SurfaceRecord(); i++) {
		R_SurfaceRecord* record = GetSurfaceRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_srid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin, GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}

	// Feature Records
	for (int i = 0; i < GetCount_FeatureTypeRecord(); i++) {
		R_FeatureRecord* record = GetFeatureRecordByIndex(i);
		if (record) {
			RecordName recordName = record->GetRecordName();
			int rcnm = recordName.RCNM;
			int rcid = recordName.RCID;
			int ruin = record->m_frid.m_ruin;
			__int64 key = MakeKey(rcnm, rcid);

			UpdateItem item(rcnm, rcid, ruin, GetRecordTypeString(rcnm),
				GenerateRecordDescription(rcnm, rcid));

			m_updateItems[key] = item;
			//m_rcnmIndex[rcnm].push_back(key);
		}
	}
}

std::string S101UpdateCell::GetRecordTypeString(int rcnm)
{
	switch (rcnm)
	{
	case 100: return "FEATURE";
	case 110: return "POINT";
	case 115: return "MULTIPOINT";
	case 120: return "CURVE";
	case 125: return "COMPOSITE";
	case 130: return "SURFACE";
	case 150: return "INFORMATION";
	default: return "UNKNOWN";
	}
}

std::string S101UpdateCell::GenerateRecordDescription(int rcnm, int rcid)
{
	std::string typeStr = GetRecordTypeString(rcnm);
	return typeStr + " Record (RCNM:" + std::to_string(rcnm) + ", RCID:" + std::to_string(rcid) + ")";
}

// 업데이트 항목 활성화/비활성화 (외부에서는 RCNM, RCID만 사용)
void S101UpdateCell::EnableUpdateItem(int rcnm, int rcid, bool enable)
{
	int64_t key = MakeKey(rcnm, rcid);
	std::unordered_map<int64_t, UpdateItem>::iterator it = m_updateItems.find(key);
	if (it != m_updateItems.end())
	{
		it->second.isEnabled = enable;
	}
	//else
	//{
	//	// 항목이 없으면 새로 생성
	//	std::string recordType = GetRecordTypeString(rcnm);
	//	std::string description = GenerateRecordDescription(rcnm, rcid);
	//	UpdateItem item(rcnm, rcid, recordType, description, enable);

	//	m_updateItems[key] = item;
	//	//m_rcnmIndex[rcnm].push_back(key);
	//}
}

// 업데이트 항목 활성화 여부 확인 (외부에서는 RCNM, RCID만 사용)
bool S101UpdateCell::IsUpdateItemEnabled(int rcnm, int rcid) const
{
	int64_t key = MakeKey(rcnm, rcid);
	std::unordered_map<int64_t, UpdateItem>::const_iterator it = m_updateItems.find(key);
	if (it != m_updateItems.end()) {
		return it->second.isEnabled;
	}
	return false;
}

bool S101UpdateCell::IsRecordEnabled(int rcnm, int rcid) const
{
	return IsUpdateItemEnabled(rcnm, rcid);
}

// 업데이트 항목 조회 (외부에서는 RCNM, RCID만 사용)
UpdateItem* S101UpdateCell::GetUpdateItem(int rcnm, int rcid)
{
	int64_t key = MakeKey(rcnm, rcid);
	std::unordered_map<int64_t, UpdateItem>::iterator it = m_updateItems.find(key);
	if (it != m_updateItems.end()) {
		return &it->second;
	}
	return nullptr;
}

// 특정 RCNM의 업데이트 항목들 조회 (O(n) - 인덱스 없이 전체 순회)
std::vector<UpdateItem*> S101UpdateCell::GetUpdateItemsByRCNM(int rcnm)
{
	std::vector<UpdateItem*> result;
	for (auto& pair : m_updateItems)
	{
		if (pair.second.rcnm == rcnm)
		{
			result.push_back(&pair.second);
		}
	}
	return result;
}

// 특정 레코드 타입의 업데이트 항목들 조회 (O(n) - 필요시만 사용)
std::vector<UpdateItem*> S101UpdateCell::GetUpdateItemsByType(const std::string& recordType)
{
	std::vector<UpdateItem*> result;
	for (auto& pair : m_updateItems)
	{
		if (pair.second.recordType == recordType)
		{
			result.push_back(&pair.second);
		}
	}
	return result;
}

// 모든 업데이트 항목 순회용 (필요시만 사용)
std::vector<UpdateItem*> S101UpdateCell::GetAllUpdateItems()
{
	std::vector<UpdateItem*> result;
	result.reserve(m_updateItems.size());
	for (auto& pair : m_updateItems)
	{
		result.push_back(&pair.second);
	}
	return result;
}

// 활성화된 업데이트 항목 개수
int S101UpdateCell::GetEnabledUpdateItemCount() const
{
	int count = 0;
	for (const auto& pair : m_updateItems) {
		if (pair.second.isEnabled) {
			count++;
		}
	}
	return count;
}

// 전체 업데이트 항목 개수
int S101UpdateCell::GetUpdateItemCount() const
{
	return static_cast<int>(m_updateItems.size());
}

// 활성화된 Information 레코드들만 반환
std::vector<R_InformationRecord*> S101UpdateCell::GetEnabledInformation()
{
	std::vector<R_InformationRecord*> result;
	for (int i = 0; i < GetCount_InformationRecord(); i++)
	{
		R_InformationRecord* record = GetInformationRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

// 활성화된 Point 레코드들만 반환
std::vector<R_PointRecord*> S101UpdateCell::GetEnabledPoint()
{
	std::vector<R_PointRecord*> result;
	for (int i = 0; i < GetCount_PointRecord(); i++)
	{
		R_PointRecord* record = GetPointRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

// 활성화된 MultiPoint 레코드들만 반환
std::vector<R_MultiPointRecord*> S101UpdateCell::GetEnabledMultiPoint()
{
	std::vector<R_MultiPointRecord*> result;
	for (int i = 0; i < GetCount_MultiPointRecord(); i++)
	{
		R_MultiPointRecord* record = GetMultiPointRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

// 활성화된 Curve 레코드들만 반환
std::vector<R_CurveRecord*> S101UpdateCell::GetEnabledCurve()
{
	std::vector<R_CurveRecord*> result;
	for (int i = 0; i < GetCount_CurveRecord(); i++)
	{
		R_CurveRecord* record = GetCurveRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

// 활성화된 Composite 레코드들만 반환
std::vector<R_CompositeRecord*> S101UpdateCell::GetEnabledComposite()
{
	std::vector<R_CompositeRecord*> result;
	for (int i = 0; i < GetCount_CompositeCurveRecord(); i++)
	{
		R_CompositeRecord* record = GetCompositeCurveRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

// 활성화된 Surface 레코드들만 반환
std::vector<R_SurfaceRecord*> S101UpdateCell::GetEnabledSurface()
{
	std::vector<R_SurfaceRecord*> result;
	for (int i = 0; i < GetCount_SurfaceRecord(); i++)
	{
		R_SurfaceRecord* record = GetSurfaceRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

// 활성화된 Feature 레코드들만 반환
std::vector<R_FeatureRecord*> S101UpdateCell::GetEnabledFeature()
{
	std::vector<R_FeatureRecord*> result;
	for (int i = 0; i < GetCount_FeatureTypeRecord(); i++)
	{
		R_FeatureRecord* record = GetFeatureRecordByIndex(i);
		if (record)
		{
			RecordName recordName = record->GetRecordName();
			if (IsRecordEnabled(recordName.RCNM, recordName.RCID))
			{
				result.push_back(record);
			}
		}
	}
	return result;
}

//// Drawing 오버라이드 메서드들
//void S101UpdateCell::Draw(HDC& hDC, Scaler* scaler, double offset)
//{
//	if (!EnableUpdate) {
//		S101Cell::Draw(hDC, scaler, offset);
//		return;
//	}
//	
//	// 활성화된 업데이트 데이터만 그리기
//	// 구체적인 구현은 기존 S101Cell::Draw 로직을 참고하여 
//	// 활성화된 레코드만 필터링하여 그리기
//	S101Cell::Draw(hDC, scaler, offset);
//}
//
//void S101UpdateCell::Draw(HDC& hDC, Scaler* scaler, int priority, int instructionType, double offset)
//{
//	if (!EnableUpdate) {
//		S101Cell::Draw(hDC, scaler, priority, instructionType, offset);
//		return;
//	}
//	
//	// 활성화된 업데이트 데이터만 그리기
//	S101Cell::Draw(hDC, scaler, priority, instructionType, offset);
//}
//
//void S101UpdateCell::Draw(D2D1Resources* D2, Scaler* scaler)
//{
//	if (!EnableUpdate) {
//		S101Cell::Draw(D2, scaler);
//		return;
//	}
//	
//	// 활성화된 업데이트 데이터만 그리기
//	S101Cell::Draw(D2, scaler);
//}
