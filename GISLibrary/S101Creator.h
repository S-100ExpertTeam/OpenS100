#pragma once

class S101Cell;
class FeatureCatalogue;
class R_FeatureRecord;

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

	R_FeatureRecord* AddAttribute(R_FeatureRecord* feature, std::wstring code, std::wstring value, int paix = 0);
	R_InformationRecord* AddAttribute(R_InformationRecord* feature, std::wstring code, std::wstring value, int paix = 0);

	R_FeatureRecord* SetGeometry(R_FeatureRecord* feature, unsigned char* value, int size);

	RecordName NewFeatureRecordName();
	RecordName NewInformationRecordName();
	RecordName NewPointRecordName();
	RecordName NewMultiPointRecordName();
	RecordName NewCurveRecordName();
	RecordName NewCompositeCurveRecordName();
	RecordName NewSurfaceRecordName();
};

