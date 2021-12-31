#pragma once

class R_FeatureRecord;
class ENC_Feature;
class FeatureType;
class ENC_Vector;
class R_PointRecord;
class SpatialObject;
class FeatureCatalogue;
class NewFeatureManager
{
public:
	NewFeatureManager();
	virtual ~NewFeatureManager();

public:
	FeatureType* m_selectedFeatureType; //Variables for storing the selected feature type
	FeatureCatalogue* m_selectedCatalog;
	SpatialObject* m_cell;
	int m_featureType;
	int m_geometryType;

public:
	R_FeatureRecord* SearchFeatureRecordHasVector(R_PointRecord *vr);
	ENC_Feature* SearchFeatureRecordHasVector(ENC_Vector *vr);
	FeatureCatalogue* GetFeatureCatalog();
	FeatureType* GetFeatureType();
	void SetFeatureType(FeatureCatalogue* _catalog, FeatureType* _ft);
};