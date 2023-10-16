#pragma once

#include "R_VectorRecord.h"
#include "F_FRID.h"
#include "F_FOID.h"
#include "OrientedCurveRecord.h"
#include "GF_FeatureType.h"
#include "GM_Object.h"

#include <map>

class SGeometry;
class SENC_Template;
class R_CurveRecord;
class R_PointRecord;
class F_ATTR;
class F_INAS;
class F_SPAS;
class F_FASC;
class F_MASK;

struct ATTR;
struct SPAS;
struct MASK;

// Feature Type Record
class R_FeatureRecord : public GF::FeatureType, public Record
{
public:
	R_FeatureRecord();
	R_FeatureRecord(const R_FeatureRecord& other);
	virtual ~R_FeatureRecord();

public:
	F_FRID m_frid;
	F_FOID m_foid;

	std::list<F_ATTR*> m_attr;
	std::list<F_INAS*> m_inas;
	std::list<F_SPAS*> m_spas;
	std::list<F_FASC*> m_fasc;
	std::list<F_MASK*> m_mask;

	//SGeometry *m_geometry = nullptr;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	void Draw(HDC &hDC, Scaler *scaler, double offset = 0);
	
	MBR GetMBR();
	int GetRCID();
	std::string GetRCIDAsString(std::string prefix = "");

	void SetRCID(int rcid);

	int GetAssociationCount();
	int GetFeatureAssociationCount();
	int GetInformationAssociationCount();
	
	int GetNumericCode();
	void SetNumericCode(int numericCode);

	// 0 : No geometry
	// 110 ~ 130 : normal
	// -1 : error
	int GetSPASRCNM();
	int GetSPASCount();

	std::vector<ATTR*> GetAllAttributes();

	std::vector<ATTR*> GetRootAttributes();
	std::vector<ATTR*> GetRootAttributes(int numericCode);

	std::vector<ATTR*> GetChildAttributes(ATTR* parentATTR);
	std::vector<ATTR*> GetChildAttributes(ATTR* parentATTR, int numericCode);

	std::vector<F_INAS*> GetAllInformationAssociations();
	std::vector<F_FASC*> GetAllFeatureAssociations();

	int GetAttributeIndex(ATTR* attr);

	SPAS* GetSPAS();
	SPAS* CreateEmptySPAS();
	void SetSPAS(int rcnm, int rcid, int ornt);

	void SetVectorRecord(R_VectorRecord* record);

	std::vector<MASK*> GetAllMASK();

public: // override ObjectType
	std::string GetID() override;
	std::wstring GetIDAsWString() override;
	int GetIDAsInteger() override;
	int GetInformationRelationCount() override;
	std::string GetAssociatedInformationID(int index) override;
	int GetAttributeCount() override;
	std::string GetAttributeValue(int index) override;
	int GetParentAttributeIndex(int index) override;

public: // override FeatureType
	bool IsNoGeometry() override;
	int GetFeatureRelationCount() override;
	std::string GetAssociatedFeatureID(int index) override;
	SpatialPrimitiveType GetSpatialPrimitiveType() override;
	GM::Object* GetGMGeometry() override;
};