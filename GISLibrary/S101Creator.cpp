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
#include "ATTR.h"
#include "F_ATTR.h"
#include "F_PTAS.h"
#include "PTAS.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../S100Geometry/SPoint.h"
#include "../S100Geometry/SMultiPoint.h"
#include "../S100Geometry/SCurve.h"
#include "../S100Geometry/SCompositeCurve.h"
#include "../S100Geometry/SSurface.h"

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
		featureRecord->m_frid = FRID;

		// FOID
		F_FOID FOID(280, 1, 1);
		featureRecord->m_foid = FOID;
		
		enc->InsertFeatureRecord(FRID.m_name.GetName(), featureRecord);

		return featureRecord;
	}

	return nullptr;
}

R_InformationRecord* S101Creator::AddInformation(std::wstring code)
{
	auto informationType = fc->GetInformationType(code);
	if (informationType)
	{
		R_InformationRecord* informationRecord = new R_InformationRecord();

		// FRID
		auto recordName = NewInformationRecordName();
		int NFTC = enc->m_dsgir.GetInformationTypeCode(code);
		F_IRID IRID(recordName, NFTC);
		informationRecord->m_irid = IRID;

		enc->InsertInformationRecord(IRID.m_name.GetName(), informationRecord);

		return informationRecord;
	}

	return nullptr;
}

ATTR* S101Creator::AddSimpleAttribute(R_FeatureRecord* feature, std::wstring code, std::wstring value)
{
	auto attribute = fc->GetSimpleAttribute(code);
	if (attribute)
	{
		if (0 == feature->m_attr.size())
		{
			feature->m_attr.push_back(new F_ATTR());
		}

		auto attributeList = feature->m_attr.front();

		auto attr = new ATTR();
		attributeList->Insert(attr);

		attr->m_natc = enc->m_dsgir.GetAttributeCode(code);
		attr->m_atix = GetATIXofNewRootAttribute(feature, attr->m_natc);
		attr->m_paix = 0;
		attr->m_atin = 1;
		attr->m_atvl = value.c_str();

		return attr;
	}

	return nullptr;
}

ATTR* S101Creator::AddSimpleAttribute(R_InformationRecord* information, std::wstring code, std::wstring value)
{
	auto attribute = fc->GetSimpleAttribute(code);
	if (attribute)
	{
		if (0 == information->m_attr.size())
		{
			information->m_attr.push_back(new F_ATTR());
		}

		auto attributeList = information->m_attr.front();

		auto attr = new ATTR();
		attributeList->Insert(attr);

		attr->m_natc = enc->m_dsgir.GetAttributeCode(code);
		attr->m_atix = GetATIXofNewRootAttribute(information, attr->m_natc);
		attr->m_paix = 0;
		attr->m_atin = 1;
		attr->m_atvl = value.c_str();

		return attr;
	}

	return nullptr;
}

ATTR* S101Creator::AddSimpleAttribute(R_FeatureRecord* feature, ATTR* parentATTR, std::wstring code, std::wstring value)
{
	if (parentATTR == nullptr)
	{
		return AddSimpleAttribute(feature, code, value);
	}

	int parentAttributeIndex = feature->GetAttributeIndex(parentATTR);
	if (parentAttributeIndex == 0)
	{
		return nullptr;
	}

	auto attribute = fc->GetSimpleAttribute(code);
	if (attribute)
	{
		auto attributeList = feature->m_attr.front();

		auto attr = new ATTR();
		attributeList->Insert(attr);

		attr->m_natc = enc->m_dsgir.GetAttributeCode(code);
		attr->m_atix = GetATIXofNewChildAttribute(feature, parentATTR, attr->m_natc);
		attr->m_paix = parentAttributeIndex;
		attr->m_atin = 1;
		attr->m_atvl = value.c_str();

		return attr;
	}

	return nullptr;
}

ATTR* S101Creator::AddSimpleAttribute(R_InformationRecord* information, ATTR* parentATTR, std::wstring code, std::wstring value)
{
	if (parentATTR == nullptr)
	{
		return AddSimpleAttribute(information, code, value);
	}

	int parentAttributeIndex = information->GetAttributeIndex(parentATTR);
	if (parentAttributeIndex == 0)
	{
		return nullptr;
	}

	auto attribute = fc->GetSimpleAttribute(code);
	if (attribute)
	{
		auto attributeList = information->m_attr.front();

		auto attr = new ATTR();
		attributeList->Insert(attr);

		attr->m_natc = enc->m_dsgir.GetAttributeCode(code);
		attr->m_atix = GetATIXofNewChildAttribute(information, parentATTR, attr->m_natc);
		attr->m_paix = parentAttributeIndex;
		attr->m_atin = 1;
		attr->m_atvl = value.c_str();

		return attr;
	}

	return nullptr;
}

SGeometry* S101Creator::SetPointGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SPoint();
	}

	feature->m_geometry->ImportFromWkb(value, size);
	
	auto geometry = (SPoint*)feature->m_geometry;
	
	auto vectorRecord = ConvertInsertVectorRecord(geometry);

	feature->SetVectorRecord(vectorRecord);

	return feature->m_geometry;
}

SGeometry* S101Creator::SetMultiPointGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SMultiPoint();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	auto geometry = (SMultiPoint*)feature->m_geometry;

	auto vectorRecord = ConvertInsertVectorRecord(geometry);

	feature->SetVectorRecord(vectorRecord);

	return feature->m_geometry;
}

SGeometry* S101Creator::SetCurveGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SCurve();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	return feature->m_geometry;
}

SGeometry* S101Creator::SetCompositeCurveGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SCompositeCurve();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	return feature->m_geometry;
}

SGeometry* S101Creator::SetSurfaceGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SSurface();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	return feature->m_geometry;
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
		auto RCID = *std::prev(RCIDs.end());
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
		auto RCID = *std::prev(RCIDs.end());
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
		auto RCID = *std::prev(RCIDs.end());
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
		auto RCID = *std::prev(RCIDs.end());
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
		auto RCID = *std::prev(RCIDs.end());
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
		auto RCID = *std::prev(RCIDs.end());
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
		auto RCID = *std::prev(RCIDs.end());
		return RecordName(GISLibrary::RCNM::Surface, RCID);
	}

	return RecordName(GISLibrary::RCNM::Surface, 1);
}

int S101Creator::GetATIXofNewRootAttribute(R_FeatureRecord* feature, int numericCode)
{
	auto rootAttributes = feature->GetRootAttributes(numericCode);

	if (rootAttributes.size() == 0)
	{
		return 1;
	}
	
	std::set<int> ATIXs;
	for (auto i = rootAttributes.begin(); i != rootAttributes.end(); i++)
	{
		ATIXs.insert((*i)->m_atix);
	}

	return (*ATIXs.end()) + 1;
}

int S101Creator::GetATIXofNewRootAttribute(R_InformationRecord* information, int numericCode)
{
	auto rootAttributes = information->GetRootAttributes(numericCode);

	if (rootAttributes.size() == 0)
	{
		return 1;
	}

	std::set<int> ATIXs;
	for (auto i = rootAttributes.begin(); i != rootAttributes.end(); i++)
	{
		ATIXs.insert((*i)->m_atix);
	}

	return (*ATIXs.end()) + 1;
}

int S101Creator::GetATIXofNewChildAttribute(R_FeatureRecord* feature, ATTR* parentATTR, int numericCode)
{
	auto childAttributes = feature->GetChildAttributes(parentATTR, numericCode);

	if (childAttributes.size() == 0)
	{
		return 1;
	}

	std::set<int> ATIXs;
	for (auto i = childAttributes.begin(); i != childAttributes.end(); i++)
	{
		ATIXs.insert((*i)->m_atix);
	}

	return (*ATIXs.end()) + 1;
}

int S101Creator::GetATIXofNewChildAttribute(R_InformationRecord* information, ATTR* parentATTR, int numericCode)
{
	auto childAttributes = information->GetChildAttributes(parentATTR, numericCode);

	if (childAttributes.size() == 0)
	{
		return 1;
	}

	std::set<int> ATIXs;
	for (auto i = childAttributes.begin(); i != childAttributes.end(); i++)
	{
		ATIXs.insert((*i)->m_atix);
	}

	return (*ATIXs.end()) + 1;
}

R_PointRecord* S101Creator::ConvertInsertVectorRecord(SPoint* geom)
{
	auto vectorRecord = new R_PointRecord();

	auto recordName = NewPointRecordName();
	vectorRecord->m_prid = F_PRID(recordName);
	vectorRecord->SetC2IT(geom->x * enc->GetCMFX(), geom->y * enc->GetCMFY());

	enc->InsertRecord(vectorRecord);

	return vectorRecord;
}

R_MultiPointRecord* S101Creator::ConvertInsertVectorRecord(SMultiPoint* geom)
{
	auto vectorRecord = new R_MultiPointRecord();

	auto recordName = NewMultiPointRecordName();
	vectorRecord->m_mrid = F_MRID(recordName);
	
	auto numPoint = geom->GetNumPoints();
	for (int i = 0; i < numPoint; i++)
	{
		vectorRecord->InsertC3IL(
			geom->GetX(i) * enc->GetCMFX(),
			geom->GetY(i) * enc->GetCMFY(),
			geom->GetZ(i) * enc->GetCMFZ());
	}

	enc->InsertRecord(vectorRecord);

	return vectorRecord;
}

R_CurveRecord* S101Creator::ConvertInsertVectorRecord(SCurveHasOrient* geom)
{
	auto vectorRecord = new R_CurveRecord();

	auto recordName = NewCurveRecordName();
	vectorRecord->m_crid = F_CRID(recordName);

	vectorRecord->m_ptas = new F_PTAS();

	if (geom->IsClosed())
	{
		auto firstPoint = geom->GetFirstPoint();
		
		if (firstPoint)
		{
			auto pointRecord = ConvertInsertVectorRecord(firstPoint);

			if (pointRecord)
			{
				auto ptas = new PTAS();
				ptas->m_name = pointRecord->GetRecordName();
				ptas->m_topi = 3;

				vectorRecord->m_ptas->m_arr.push_back(ptas);
			}
		}
	}
	else
	{
		auto firstPoint = geom->GetFirstPoint();
		auto lastPoint = geom->GetLastPoint();

		if (firstPoint && lastPoint)
		{
			auto firstPointRecord = ConvertInsertVectorRecord(firstPoint);
			auto lastPointRecord = ConvertInsertVectorRecord(lastPoint);

			if (firstPointRecord && lastPointRecord)
			{
				auto firstPointPTAS = new PTAS();
				firstPointPTAS->m_name = firstPointRecord->GetRecordName();
				firstPointPTAS->m_topi = 3;
				vectorRecord->m_ptas->m_arr.push_back(firstPointPTAS);

				auto lastPointPTAS = new PTAS();
				lastPointPTAS->m_name = lastPointRecord->GetRecordName();
				lastPointPTAS->m_topi = 3;
				vectorRecord->m_ptas->m_arr.push_back(lastPointPTAS);
			}
		}
	}

	//vectorRecord->m_ptas->m_arr.resize(numPTAS);

	//for (int i = 0; i < numPTAS; i++)
	//{
	//	auto sPoint = geom->GetPoint(i + 1);
	//	if (sPoint)
	//	{
	//		auto pointRecord = ConvertInsertVectorRecord(sPoint);
	//		if (pointRecord)
	//		{
	//			pointRecord->
	//		}
	//	}
	//}

	//auto numPoint = geom->GetNumPoints();
	//for (int i = 0; i < numPoint; i++)
	//{
	//	vectorRecord->InsertC3IL(
	//		geom->GetX(i) * enc->GetCMFX(),
	//		geom->GetY(i) * enc->GetCMFY(),
	//		geom->GetZ(i) * enc->GetCMFZ());
	//}

	//enc->InsertRecord(vectorRecord);

	return vectorRecord;
}