#include "stdafx.h"
#include "NewFeatureManager.h"
#include "F_SPAS.h"
#include "R_FeatureRecord.h"
#include "R_PointRecord.h"
#include "SPAS.h"
#include "SpatialObject.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\FeatureCatalog\\FeatureType.h"

NewFeatureManager::NewFeatureManager()
{
	m_selectedFeatureType = NULL;
}

NewFeatureManager::~NewFeatureManager()
{
}

FeatureCatalogue* NewFeatureManager::GetFeatureCatalog()
{
	return m_selectedCatalog;
}

FeatureType* NewFeatureManager::GetFeatureType()
{
	return m_selectedFeatureType;
}

void NewFeatureManager::SetFeatureType(FeatureCatalogue* _fc, FeatureType* _ft)
{
	if(_fc)
		m_selectedCatalog = _fc;
	m_selectedFeatureType = _ft;
}