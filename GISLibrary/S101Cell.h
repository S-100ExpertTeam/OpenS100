#pragma once

#include "S100SpatialObject.h"
#include "R_DSGIR.h"
#include "R_DSCRS.h"
#include "F_CodeWithNumericCode.h"
#include "DDR.h"

#include "..\\GeoMetryLibrary\\MBR.h"

#include <unordered_map>
#include <set>
#include <map>

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
class SCurveHasOrient;
class F_INAS;
class F_C2IL;
class F_C3IL;
class F_COCC;
class F_ATTR;

struct ATTR;

// Class for saving 000 information.
// Base and Update save
class S101Cell : public S100SpatialObject
{
public:
	S101Cell();
	virtual ~S101Cell();

public:
	DDR m_S101DDR;

	S101Cell* updateInformation = nullptr;

	// only have the base.
	std::vector<S101Cell*> updates;

	// Dataset General Information Record 
	R_DSGIR m_dsgir;

	// Dataset Coordinate Reference System Record 
	// Base : O
	// Update : X
	// Comment : It's a record that doesn't exist when it's an update file, so you need to change it to a pointer
	R_DSCRS m_dscrs;

	std::unordered_map<__int64, SCurve*> m_curveMap;
	std::unordered_map<__int64, R_VectorRecord*> m_vecMap;

	// When S101Cell stores GML, a variable for storing Namespace.
	std::unordered_map<std::wstring, std::wstring>* gml_namespace = nullptr;

private:
	CMap<__int64, __int64, R_InformationRecord*, R_InformationRecord*&> m_infMap; // Information Type
	CMap<__int64, __int64, R_PointRecord*, R_PointRecord*&> m_ptMap;              // Point
	CMap<__int64, __int64, R_MultiPointRecord*, R_MultiPointRecord*&> m_mpMap;    // Multi Point
	CMap<__int64, __int64, R_CurveRecord*, R_CurveRecord*&> m_curMap;             // Curve
	CMap<__int64, __int64, R_CompositeRecord*, R_CompositeRecord*&> m_comMap;     // Composite Curve
	CMap<__int64, __int64, R_SurfaceRecord*, R_SurfaceRecord*&> m_surMap;         // Surface
	CMap<__int64, __int64, R_FeatureRecord*, R_FeatureRecord*&> m_feaMap;         // Feature Type

	std::vector<R_InformationRecord*> vecInformation;
	std::vector<R_PointRecord*> vecPoint;
	std::vector<R_MultiPointRecord*> vecMultiPoint;
	std::vector<R_CurveRecord*> vecCurve;
	std::vector<R_CompositeRecord*> vecComposite;
	std::vector<R_SurfaceRecord*> vecSurface;
	std::vector<R_FeatureRecord*> vecFeature;

public:
	R_DSGIR* GetDatasetGeneralInformationRecord();
	void UpdateRemoveAll(void);
	void RemoveAll(void);
	void ClearAll(void);
	bool Open(CString _filepath);
	BOOL ReadDDR(BYTE*& buf);
	void SortByFeatureType();
	void GetAllFeatureDisplayOptions();
	void GetFeatureDisplayOption(R_FeatureRecord* pFe);
	void ProcessCSProcedure();
	void SetInstructionToFeature();
	void SetInstructionToFeature(int type, R_FeatureRecord* fe);

	void ClearCurveMap();

	BOOL MakeFullSpatialData();
	BOOL MakePointData(R_FeatureRecord* fe); 
	BOOL MakeSoundingData(R_FeatureRecord* fe);
	BOOL MakeLineData(R_FeatureRecord* fe);
	BOOL MakeAreaData(R_FeatureRecord* fe);

	BOOL GetFullSpatialData(R_PointRecord *r, SPoint* point);
	BOOL GetFullSpatialData(R_PointRecord *r, GeoPointZ &geo);
	BOOL GetFullSpatialData(R_MultiPointRecord *r, CArray<GeoPointZ> &geoArr);
	BOOL GetFullSpatialData(R_CurveRecord *r, CArray<GeoPoint> &geoArr, int ORNT = 1);
	BOOL GetFullSpatialData(R_CurveRecord *r, std::vector<POINT> &geoArr, int ORNT = 1);
	BOOL GetFullSpatialData(R_CompositeRecord *r, CArray<GeoPoint> &geoArr, int ORNT = 1);
	BOOL GetFullSpatialData(R_CompositeRecord *r, std::vector<POINT> &geoArr, int ORNT = 1);
	BOOL GetFullSpatialData(R_SurfaceRecord *r, CArray<GeoPoint> &geoArr);

	SCurve* GetCurveGeometry(R_CurveRecord *r/*, CArray<GeoPoint> &geoArr, unsigned ORNT = 1*/);
	BOOL SetSCurveList(std::list<OrientedCurveRecord>* inCurveRecordList, std::list<SCurveHasOrient>* outSCurveList);

	BOOL GetFullCurveData(R_FeatureRecord* fe, R_CurveRecord *r, int ornt = 1);
	BOOL GetFullCurveData(R_FeatureRecord* fe, R_CompositeRecord *r, int ornt = 1);
	BOOL GetFullCurveData(R_FeatureRecord* fe, R_SurfaceRecord *r, int ornt = 1);

	BOOL GetFullMaskData(R_FeatureRecord* fe);

	void Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler);
	void Draw(HDC &hDC, Scaler *scaler, double offset = 0);
	/*
	* set instruction type 
	* 1 : Point
	* 2 : Curve
	* 3 : Surface 
	* 4 : Text
	* 5 : Multi Point (Sounding)
	*/
	void Draw(HDC &hDC, Scaler *scaler, int priority, int instructionType, double offset = 0);

	MBR CalcMBR();


	//void SaveTextPlacement(CString _filepath, CString extend);
	void GetDDRFromFile(CString _filepath);

	void ProcessSpatialReference();
	
	// [ Text Placement ]
	void CheckHasTextPlacement();

	void SetEncodingSpecification(CString value);
	CString GetEncodingSpecification();
	std::string GetEncodingSpecificationToString();

	void SetEncodingSpecificationEdition(CString value);
	CString GetEncodingSpecificationEdition();
	std::string GetEncodingSpecificationEditionToString();

	void SetProductIdentifier(CString value);
	CString GetProductIdentifier();
	std::string GetProductIdentifierToString();

	void SetProductEdition(CString value);
	CString GetProductEdition();
	std::string GetProductEditionToString();
	
	void SetApplictionProfile(CString value);
	CString GetApplicationProfile();
	std::string GetApplicationProfileToString();

	void SetDatasetFileIdentifier(CString value);
	CString GetDatasetFileIdentifier();
	std::string GetDatasetFileIdentifierToString();

	void SetDatasetTitle(CString value);
	CString GetDatasetTitle();
	std::string GetDatasetTitleToString();

	void SetDatasetReferenceDate(CString value);
	CString GetDatasetReferenceDate();
	std::string GetDatasetReferenceDataToString();

	void SetDatasetLanguage(CString value);
	CString GetDatasetLanguage();
	std::string GetDatasetLanguageToString();

	void SetDatasetAbstract(CString value);
	CString GetDatasetAbstract();
	std::string GetDatasetAbstractToString();

	void SetDatasetEdition(CString value);
	CString GetDatasetEdition();
	std::string GetDatasetEditionToString();


	//============================================================================//
	void InsertInformationRecord(__int64 key, R_InformationRecord* record);
	void RemoveInformationRecord(__int64 key, R_InformationRecord* record);
	R_InformationRecord* GetInformationRecord(__int64 key);
	POSITION GetInfoStartPosition();
	void GetNextAssoc(POSITION& index, long long& key, R_InformationRecord*& value);
	void RemoveAllInfoRecord();
	int GetInfoMapCount();
	//vectorinformation
	std::vector<R_InformationRecord*> GetVecInformation();


	void InsertPointRecord(__int64 key, R_PointRecord* record);
	void RemovePointRecord(__int64 key, R_PointRecord* record);
	R_PointRecord* GetPointRecord(__int64 key);
	POSITION GetpointStartPosition();
	void GetNextAssoc(POSITION& index, long long& key, R_PointRecord*& value);
	void RemoveAllPointRecord();
	//vecpoint
	std::vector<R_PointRecord*> GetVecPoint();


	void InsertMultiPointRecord(__int64 key, R_MultiPointRecord* record);
	void RemoveMultiPointRecord(__int64 key, R_MultiPointRecord* record);
	R_MultiPointRecord* GetMultiPointRecord(__int64 key);
	POSITION GetMultStartPosition();
	void GetNextAssoc(POSITION& index, long long& key, R_MultiPointRecord*& value);
	void RemoveAllMultRecord();
	//vecMult
	std::vector<R_MultiPointRecord*> GetVecMultiPoint();


	void InsertCurveRecord(__int64 key, R_CurveRecord* record);
	void RemoveCurveRecord(__int64 key, R_CurveRecord* record);
	R_CurveRecord* GetCurveRecord(__int64 key);
	POSITION GetCurStartPosition();
	void GetNextAssoc(POSITION& index, long long& key,R_CurveRecord*& value);
	void RemoveAllCurRecord();
	//veccurve
	std::vector<R_CurveRecord*> GetVecCurve();


	void InsertCompositeCurveRecord(__int64 key, R_CompositeRecord* record);
	void RemoveCompositeCurveRecord(__int64 key, R_CompositeRecord* record);

	R_CompositeRecord* GetCompositeCurveRecord(__int64 key);
	POSITION GetComStartPosition();
	void GetNextAssoc(POSITION& index, long long& key, R_CompositeRecord*& value);
	void RemoveAllComRecord();
	std::vector<R_CompositeRecord*> GetVecComposite();


	void InsertSurfaceRecord(__int64 key, R_SurfaceRecord* record);
	void RemoveSurfaceRecord(__int64 key, R_SurfaceRecord* record);
	R_SurfaceRecord* GetSurfaceRecord(__int64 key);
	POSITION GetSurStartPosition();
	void GetNextAssoc(POSITION& index, long long& key, R_SurfaceRecord*& value);
	void RemoveAllSurRecord();
	std::vector<R_SurfaceRecord*> GetVecSurface();

	void InsertFeatureRecord(__int64 key, R_FeatureRecord* record);
	void RemoveFeatureRecord(__int64 key, R_FeatureRecord* record);

	R_FeatureRecord* GetFeatureRecord(__int64 key);
	POSITION GetFeatureStartPosition();
	void GetNextAssoc(POSITION& index,long long& key,R_FeatureRecord*& value);
	void RemoveFeatureMapKey(long long key);
	void RemoveAllFeatureRecord();
	std::vector<R_FeatureRecord*> GetVecFeature();

	//============================================================================//
	int GetCount_InformationRecord();
	int GetCount_PointRecord();
	int GetCount_MultiPointRecord();
	int GetCount_CurveRecord();
	int GetCount_CompositeCurveRecord();
	int GetCount_SurfaceRecord();
	int GetCount_FeatureTypeRecord();

	// The number of records in the DSSI field.
	int GetMetaCount_InformationRecord();
	int GetMetaCount_PointRecord();
	int GetMetaCount_MultiPointRecord();
	int GetMetaCount_CurveRecord();
	int GetMetaCount_CompositeCurveRecord();
	int GetMetaCount_SurfaceRecord();
	int GetMetaCount_FeatureTypeRecord();
	bool Check();
	bool Update(S101Cell* cell);
	bool IsUpdate();

private:
	bool UpdateDsgirRecord(S101Cell* cell);
	bool UpdateAttrField(std::list<F_ATTR*> Update, F_CodeWithNumericCode* atcs);
	bool UpdateInasField(std::list<F_INAS*> Update, F_CodeWithNumericCode* atcs);

	// Function for adding S-101 update function.
	bool UpdateInfMapRecord(S101Cell* cell);
	bool UpdateAttrRecord(std::list<F_ATTR*> Update, std::list<F_ATTR*> Base) ; // To increase utilization, change parameters.
	bool UpdateINASRecord(std::list<F_INAS*> Update, std::list<F_INAS*> Base);


	bool UpdateC2ILRecord(std::list<F_C2IL*> update, std::list<F_C2IL*> base, F_COCC* updatemission);
	bool UpdateC3ILRecord(std::list<F_C3IL*> update, std::list<F_C3IL*> base);
	bool UpdatePtMapRecord(S101Cell* cell);
	bool UpdateMpMapRecord(S101Cell* cell);
	bool UpdateCurMapRecord(S101Cell* cell);
	bool UpdateComMapRecord(S101Cell* cell);
	bool UpdateSurMapRecord(S101Cell* cell);
	bool UpdateFeaMapRecord(S101Cell* cell);
};