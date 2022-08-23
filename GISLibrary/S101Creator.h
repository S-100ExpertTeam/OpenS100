#pragma once

class S101Cell;
class FeatureCatalogue;
class R_FeatureRecord;
class SGeometry;

class S101Creator
{
public:
	S101Creator(FeatureCatalogue* fc, S101Cell* enc);
	~S101Creator();

public:
	FeatureCatalogue* fc = nullptr;
	S101Cell* enc = nullptr;

public:
	R_FeatureRecord* AddFeature(std::wstring code);
	R_InformationRecord* AddInformation(std::wstring code);

	ATTR* AddSimpleAttribute(R_FeatureRecord* feature, std::wstring code, std::wstring value);
	ATTR* AddSimpleAttribute(R_InformationRecord* information, std::wstring code, std::wstring value);

	ATTR* AddSimpleAttribute(R_FeatureRecord* feature, ATTR* parentATTR, std::wstring code, std::wstring value);
	ATTR* AddSimpleAttribute(R_InformationRecord* information, ATTR* parentATTR, std::wstring code, std::wstring value);

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
};

