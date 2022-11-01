#pragma once

#include "../FeatureCatalog/AttributeBinding.h"

#include <vector>

class S101Layer;
class S101Cell;
class FeatureCatalogue;
class R_FeatureRecord;
class SGeometry;

class S101Creator
{
public:
	S101Creator();
	S101Creator(FeatureCatalogue* fc, S101Cell* enc);
	~S101Creator();

public:
	FeatureCatalogue* fc = nullptr;
	S101Cell* enc = nullptr;

public:
	void Set(FeatureCatalogue* fc, S101Cell* enc);

	void SetFC(FeatureCatalogue* fc);

	S101Cell* CreateENC(std::wstring name);
	S101Layer* CreateLayer(std::wstring name, FeatureCatalogue* fc, PortrayalCatalogue* pc);

	R_FeatureRecord* AddFeature(std::wstring code);
	R_InformationRecord* AddInformation(std::wstring code);

	bool DeleteFeature(int rcid);

	ATTR* AddSimpleAttribute(R_FeatureRecord* feature, std::wstring code, std::wstring value);
	ATTR* AddSimpleAttribute(R_InformationRecord* information, std::wstring code, std::wstring value);

	ATTR* AddSimpleAttribute(R_FeatureRecord* feature, ATTR* parentATTR, std::wstring code, std::wstring value);
	ATTR* AddSimpleAttribute(R_InformationRecord* information, ATTR* parentATTR, std::wstring code, std::wstring value);

	ATTR* AddComplexAttribute(R_FeatureRecord* feature, std::wstring code);
	ATTR* AddComplexAttribute(R_InformationRecord* information, std::wstring code);

	ATTR* AddComplexAttribute(R_FeatureRecord* feature, ATTR* parentATTR, std::wstring code);
	ATTR* AddComplexAttribute(R_InformationRecord* information, ATTR* parentATTR, std::wstring code);

	SGeometry* SetPointGeometry(R_FeatureRecord* feature, unsigned char* value, int size);
	SGeometry* SetMultiPointGeometry(R_FeatureRecord* feature, unsigned char* value, int size);
	SGeometry* SetCurveGeometry(R_FeatureRecord* feature, unsigned char* value, int size);
	SGeometry* SetCompositeCurveGeometry(R_FeatureRecord* feature, unsigned char* value, int size);
	SGeometry* SetSurfaceGeometry(R_FeatureRecord* feature, unsigned char* value, int size);

	RecordName NewFeatureRecordName();
	RecordName NewInformationRecordName();
	RecordName NewPointRecordName();
	RecordName NewMultiPointRecordName();
	RecordName NewCurveRecordName();
	RecordName NewCompositeCurveRecordName();
	RecordName NewSurfaceRecordName();

	int GetATIXofNewRootAttribute(R_FeatureRecord* feature, int numericCode);
	int GetATIXofNewRootAttribute(R_InformationRecord* information, int numericCode);

	int GetATIXofNewChildAttribute(R_FeatureRecord* feature, ATTR* parentATTR, int numericCode);
	int GetATIXofNewChildAttribute(R_InformationRecord* information, ATTR* parentATTR, int numericCode);

	R_PointRecord* ConvertInsertVectorRecord(SPoint* geom);
	R_MultiPointRecord* ConvertInsertVectorRecord(SMultiPoint* geom);
	R_CurveRecord* ConvertInsertVectorRecord(SCurveHasOrient* geom);
	R_CompositeRecord* ConvertInsertVectorRecord(SCompositeCurve* geom);
	R_SurfaceRecord* ConvertInsertVectorRecord(SSurface* geom);
	

	std::list<AttributeBinding*> GetAddableAttributes(R_FeatureRecord* feature);
	std::list<AttributeBinding*> GetAddableAttributes(R_FeatureRecord* feature, ATTR* parentATTR);

	void AddDefaultChildAttributes(R_FeatureRecord* feature);
	void AddDefaultChildAttributes(R_FeatureRecord* feature, ATTR* attr);
};

