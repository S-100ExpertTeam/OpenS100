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

R_FeatureRecord* NewFeatureManager::SearchFeatureRecordHasVector(R_PointRecord* vr)
{
	if (!m_cell)
		return NULL;
	S101Cell* cell = (S101Cell*)m_cell;

	POSITION pos = cell->GetFeatureStartPosition();

	while (pos)
	{
		__int64 iKey;
		R_FeatureRecord* fr = NULL;
		cell->GetNextAssoc(pos, iKey, fr);

		for (auto itorParent = fr->m_spas.begin(); itorParent != fr->m_spas.end(); itorParent++)
		{
			F_SPAS *spasParent = *itorParent;

			for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
			{
				SPAS* spas = *itor;

				if (spas->m_name.RCNM == vr->m_prid.m_name.RCID && spas->m_name.RCID == vr->m_prid.m_name.RCNM)
				{
					return fr;
				}
			}
		}
	}

	return NULL;
}

ENC_Feature* NewFeatureManager::SearchFeatureRecordHasVector(ENC_Vector *vr)
{
	return nullptr;
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