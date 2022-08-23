#include "stdafx.h"
#include "S101Creator.h"
#include "S101Cell.h"
#include "R_FeatureRecord.h"
#include "R_InformationRecord.h"
#include "R_PointRecord.h"
#include "R_MultiPointRecord.h"
#include "R_CurveRecord.h"
#include "R_CompositeRecord.h"
#include "R_SurfaceRecord.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include <set>

S101Creator::S101Creator(FeatureCatalogue* fc, S101Cell* enc)
{
	this->fc = fc;
	this->enc = enc;
}

S101Creator::~S101Creator()
{

}

R_FeatureRecord* S101Creator::AddFeature(std::wstring code)
{
	auto featureType = fc->GetFeatureType(code);
	if (featureType)
	{
		R_FeatureRecord* featureRecord = new R_FeatureRecord();

		// FRID
		auto recordName = NewFeatureRecordName();
		int NFTC = enc->m_dsgir.GetFeatureTypeCode(code);
		F_FRID FRID(recordName, NFTC);

		// FOID
		F_FOID FOID;
	}

	return nullptr;
}

RecordName S101Creator::NewFeatureRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecFeature();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::FeatureType, RCID);
	}

	return RecordName(GISLibrary::RCNM::FeatureType, 1);
}

RecordName S101Creator::NewInformationRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecInformation();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::InformationType, RCID);
	}

	return RecordName(GISLibrary::RCNM::InformationType, 1);
}

RecordName S101Creator::NewPointRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecPoint();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::Point, RCID);
	}

	return RecordName(GISLibrary::RCNM::Point, 1);
}

RecordName S101Creator::NewMultiPointRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecMultiPoint();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::MultiPoint, RCID);
	}

	return RecordName(GISLibrary::RCNM::MultiPoint, 1);
}

RecordName S101Creator::NewCurveRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecCurve();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::Curve, RCID);
	}

	return RecordName(GISLibrary::RCNM::Curve, 1);
}

RecordName S101Creator::NewCompositeCurveRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecComposite();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::CompositeCurve, RCID);
	}

	return RecordName(GISLibrary::RCNM::CompositeCurve, 1);
}

RecordName S101Creator::NewSurfaceRecordName()
{
	std::set<int> RCIDs;
	auto item = enc->GetVecSurface();

	for (auto i = item.begin(); i != item.end(); i++)
	{
		RCIDs.insert((*i)->GetRCID());
	}

	if (RCIDs.size() > 0)
	{
		auto RCID = *RCIDs.end();
		return RecordName(GISLibrary::RCNM::Surface, RCID);
	}

	return RecordName(GISLibrary::RCNM::Surface, 1);
}