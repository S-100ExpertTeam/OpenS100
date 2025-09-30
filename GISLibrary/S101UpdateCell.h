#pragma once
#include "S101Cell.h"

#include <unordered_map>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstdint>


#include "S100SpatialObject.h"
#include "R_DSGIR.h"
#include "R_DSCRS.h"
#include "F_CodeWithNumericCode.h"
#include "DDR.h"
#include "GML_DatasetIdentificationInformation.h"
#include "S10XGML.h"
#include "RecordName.h"
#include "Enum_RCNM.h"

#include "../GeoMetryLibrary/MBR.h"

#include "../FeatureCatalog/Version.h"




class FeatureCatalogue;
class GeoPoint;
class GeoPointZ;
class Catalog;
class PCOutputSchemaManager;
class SPoint;
class SCurve;
class SSurface;
class SMultiPoint;
class R_InformationRecord;
class R_FeatureRecord;
class R_PointRecord;
class R_MultiPointRecord;
class R_CurveRecord;
class R_CompositeRecord;
class R_SurfaceRecord;
class R_VectorRecord;
class SCompositeCurve;
class TiXmlElement;
class OrientedCurveRecord;
class SCurve;
class F_FASC;
class F_INAS;
class F_C2IL;
class F_C3IL;
class F_COCC;
class F_ATTR;
class SENC_PointInstruction;
class S101Creator;

struct ATTR;
struct SPAS;

// 간단한 업데이트 항목 정보
struct UpdateItem
{
    int rcnm;                   // Record Name (레코드 타입)
    int rcid;                   // Record Identifier
    int updateInfo;
    std::string recordType;     // "INFORMATION", "POINT", "MULTIPOINT", "CURVE", "COMPOSITE", "SURFACE", "FEATURE"
    std::string description;    // 업데이트 설명
    bool isEnabled;             // 이 업데이트를 사용할지 여부

    UpdateItem() : rcnm(0), rcid(0), updateInfo(0), recordType(""), description(""), isEnabled(true) {}

    UpdateItem(int _rcnm, int _rcid, int _updateInfo, const std::string& recType, const std::string& desc, bool enabled = true)
        : rcnm(_rcnm), rcid(_rcid), updateInfo(_updateInfo),recordType(recType), description(desc), isEnabled(enabled) {
    }
};

class S101UpdateCell : public S101Cell
{
public:
    S101UpdateCell(D2D1Resources* d2d1);
    S101UpdateCell(FeatureCatalogue* fc, D2D1Resources* d2d1);
    virtual ~S101UpdateCell();

public:
    bool EnableUpdate = true;

private:
    // 내부적으로 조합키 사용 (성능 최적화)
    inline int64_t MakeKey(int rcnm, int rcid) const {
        return ((int64_t)rcnm << 32) | (int64_t)rcid;
    }

    // 효율적인 업데이트 항목 관리 (O(1) 접근)
    std::unordered_map<int64_t, UpdateItem> m_updateItems; // 조합키 -> UpdateItem

    //// 타입별 빠른 검색을 위한 인덱스
    //std::unordered_map<int, std::vector<int64_t>> m_rcnmIndex; // RCNM -> 조합키 list

public:
    void RemoveAll(void);
    void ClearAll(void);

    // 업데이트 항목 관리 (외부에서는 RCNM + RCID만 사용)
    void EnableUpdateItem(int rcnm, int rcid, bool enable);
    bool IsUpdateItemEnabled(int rcnm, int rcid) const;

    // 업데이트 정보 조회 (O(1) 접근)
    UpdateItem* GetUpdateItem(int rcnm, int rcid);
    int GetUpdateItemCount() const;
    int GetEnabledUpdateItemCount() const; // 활성화된 업데이트 항목 개수

    // 특정 레코드 타입의 업데이트 항목들 조회
    std::vector<UpdateItem*> GetUpdateItemsByRCNM(int rcnm);
    std::vector<UpdateItem*> GetUpdateItemsByType(const std::string& recordType);

    // 모든 업데이트 항목 순회용 (필요시만 사용)
    std::vector<UpdateItem*> GetAllUpdateItems();

    // 업데이트 항목 생성 (로드된 모든 데이터를 업데이트 항목으로)
    void CreateUpdateItems();

    // 활성화된 업데이트 데이터만 반환
    std::vector<R_InformationRecord*> GetEnabledInformation();
    std::vector<R_PointRecord*> GetEnabledPoint();
    std::vector<R_MultiPointRecord*> GetEnabledMultiPoint();
    std::vector<R_CurveRecord*> GetEnabledCurve();
    std::vector<R_CompositeRecord*> GetEnabledComposite();
    std::vector<R_SurfaceRecord*> GetEnabledSurface();
    std::vector<R_FeatureRecord*> GetEnabledFeature();

    //// Drawing 오버라이드 - 선택된 버전으로 렌더링
    //void Draw(HDC& hDC, Scaler* scaler, double offset = 0) override;
    //void Draw(HDC& hDC, Scaler* scaler, int priority, int instructionType, double offset = 0) override;
    //void Draw(D2D1Resources* D2, Scaler* scaler) override;

private:
    // 내부 헬퍼 메서드들
    std::string GetRecordTypeString(int rcnm);
    std::string GenerateRecordDescription(int rcnm, int rcid);

    // 레코드별 활성화 상태 확인 헬퍼 (O(1))
    bool IsRecordEnabled(int rcnm, int rcid) const;
};