#include "stdafx.h"
#include "S101Creator.h"
#include "S101Cell.h"
#include "S101Layer.h"
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
#include "F_SEGH.h"
#include "F_C2IL.h"
#include "F_CUCO.h"
#include "F_RIAS.h"
#include "F_FASC.h"
#include "IC2D.h"
#include "PTAS.h"
#include "CUCO.h"
#include "RIAS.h"
#include "SPAS.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../S100Geometry/SPoint.h"
#include "../S100Geometry/SMultiPoint.h"
#include "../S100Geometry/SCurve.h"
#include "../S100Geometry/SCompositeCurve.h"
#include "../S100Geometry/SSurface.h"

#include <set>

S101Creator::S101Creator()
{

}

S101Creator::S101Creator(FeatureCatalogue* fc, S101Cell* enc)
{
	this->fc = fc;
	this->enc = enc;
}

S101Creator::~S101Creator()
{

}

void S101Creator::Set(FeatureCatalogue* fc, S101Cell* enc)
{
	this->fc = fc;
	this->enc = enc;
}

void S101Creator::SetFC(FeatureCatalogue* fc)
{
	this->fc = fc;
}

S101Cell* S101Creator::CreateENC(std::wstring name)
{
	auto result = new S101Cell();
	enc = result;

	enc->m_dsgir.m_dsid.m_dsnm = name.c_str();

	return result;
}

S101Layer* S101Creator::CreateLayer(std::wstring name, FeatureCatalogue* fc, PortrayalCatalogue* pc)
{
	auto result = new S101Layer(fc, pc);

	result->m_spatialObject = CreateENC(name);

	return result;
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
		
		AddDefaultChildAttributes(featureRecord);

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

bool S101Creator::DeleteFeature(int rcid)
{
	RecordName featureRecordName(100, rcid);
	auto key = featureRecordName.GetName();
	auto featureRecord = enc->GetFeatureRecord(key);
	if (featureRecord)
	{
		enc->RemoveFeatureRecord(key);

		auto vecFeature = enc->GetVecFeature();
		for (auto i = vecFeature.begin(); i != vecFeature.end(); i++)
		{
			auto fr = (*i);
			for (auto j = fr->m_fasc.begin(); j != fr->m_fasc.end(); )
			{
				if (key == (*j)->m_name.GetName())
				{
					delete (*j);
					j = fr->m_fasc.erase(j);
				}
				else
				{
					j++;
				}
			}
		}

		return true;
	}

	return false;
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

ATTR* S101Creator::AddComplexAttribute(R_FeatureRecord* feature, std::wstring code)
{
	auto attribute = fc->GetComplexAttribute(code);

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
		attr->m_atvl = L"";

		return attr;
	}

	return nullptr;
}

ATTR* S101Creator::AddComplexAttribute(R_InformationRecord* information, std::wstring code)
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
		attr->m_atvl = L"";

		return attr;
	}

	return nullptr;
}

ATTR* S101Creator::AddComplexAttribute(R_FeatureRecord* feature, ATTR* parentATTR, std::wstring code)
{
	if (parentATTR == nullptr)
	{
		return AddComplexAttribute(feature, code);
	}

	int parentAttributeIndex = feature->GetAttributeIndex(parentATTR);
	if (parentAttributeIndex == 0)
	{
		return nullptr;
	}

	auto attribute = fc->GetComplexAttribute(code);
	if (attribute)
	{
		auto attributeList = feature->m_attr.front();

		auto attr = new ATTR();
		attributeList->Insert(attr);

		attr->m_natc = enc->m_dsgir.GetAttributeCode(code);
		attr->m_atix = GetATIXofNewChildAttribute(feature, parentATTR, attr->m_natc);
		attr->m_paix = parentAttributeIndex;
		attr->m_atin = 1;
		attr->m_atvl = L"";

		return attr;
	}

	return nullptr;
}

ATTR* S101Creator::AddComplexAttribute(R_InformationRecord* information, ATTR* parentATTR, std::wstring code)
{
	if (parentATTR == nullptr)
	{
		return AddComplexAttribute(information, code);
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
		attr->m_atvl = L"";

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
		feature->m_geometry = new SCurveHasOrient();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	auto geometry = (SCurveHasOrient*)feature->m_geometry;

	auto vectorRecord = ConvertInsertVectorRecord(geometry);

	feature->SetVectorRecord(vectorRecord);

	((SCurveHasOrient*)feature->m_geometry)->SetRCID(vectorRecord->GetRCID());

	return feature->m_geometry;
}

SGeometry* S101Creator::SetCompositeCurveGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SCompositeCurve();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	auto geometry = (SCompositeCurve*)feature->m_geometry;

	auto vectorRecord = ConvertInsertVectorRecord(geometry);

	feature->SetVectorRecord(vectorRecord);

	return feature->m_geometry;
}

SGeometry* S101Creator::SetSurfaceGeometry(R_FeatureRecord* feature, unsigned char* value, int size)
{
	if (feature->m_geometry == nullptr)
	{
		feature->m_geometry = new SSurface();
	}

	feature->m_geometry->ImportFromWkb(value, size);

	auto geometry = (SSurface*)feature->m_geometry;

	auto vectorRecord = ConvertInsertVectorRecord(geometry);

	feature->SetVectorRecord(vectorRecord);

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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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
		auto RCID = *std::prev(RCIDs.end()) + 1;
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

	return *ATIXs.rbegin() + 1;
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

	return *ATIXs.rbegin() + 1;
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

	double x = geom->x;
	double y = geom->y;

	inverseProjection(x, y);

	x *= enc->GetCMFX();
	y *= enc->GetCMFY();

	vectorRecord->SetC2IT(x, y);

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
		double x = geom->GetX(i);
		double y = geom->GetY(i);
		inverseProjection(x, y);
		x *= enc->GetCMFX();
		y *= enc->GetCMFY();

		vectorRecord->InsertC3IL(
			x, 
			y,
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

	int numC2IL = 0;

	if (geom->IsClosed())
	{
		numC2IL = geom->GetNumPoints() - 1;

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
		numC2IL = geom->GetNumPoints() - 2;

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
				firstPointPTAS->m_topi = 1;
				vectorRecord->m_ptas->m_arr.push_back(firstPointPTAS);

				auto lastPointPTAS = new PTAS();
				lastPointPTAS->m_name = lastPointRecord->GetRecordName();
				lastPointPTAS->m_topi = 2;
				vectorRecord->m_ptas->m_arr.push_back(lastPointPTAS);
			}
		}
	}

	vectorRecord->m_segh = new F_SEGH();

	auto f_C2IL = new F_C2IL();
	vectorRecord->m_c2il.push_back(f_C2IL);

	for (int i = 0; i < numC2IL; i++)
	{
		auto sPoint = geom->GetPoint(i + 1);
		if (sPoint)
		{
			auto C2IL = new IC2D();

			double x = sPoint->x;
			double y = sPoint->y;
			inverseProjection(x, y);
			x *= enc->GetCMFX();
			y *= enc->GetCMFY();

			C2IL->m_xcoo = x;
			C2IL->m_ycoo = y;
			f_C2IL->m_arr.push_back(C2IL);
		}
	}

	enc->InsertRecord(vectorRecord);

	return vectorRecord;
}

R_CompositeRecord* S101Creator::ConvertInsertVectorRecord(SCompositeCurve* geom)
{
	auto vectorRecord = new R_CompositeRecord();

	auto recordName = NewCompositeCurveRecordName();
	vectorRecord->m_ccid = F_CCID(recordName);

	auto f_CUCO = new F_CUCO();
	for (auto i = geom->m_listCurveLink.begin(); i != geom->m_listCurveLink.end(); i++)
	{
		auto curveRecord = ConvertInsertVectorRecord(*i);
		auto cuco = new CUCO();
		cuco->m_name = curveRecord->GetRecordName();
		cuco->m_ornt = 1;
		f_CUCO->m_arr.push_back(cuco);

		(*i)->SetRCID(curveRecord->GetRCID());
	}
	vectorRecord->m_cuco.push_back(f_CUCO);

	enc->InsertRecord(vectorRecord);

	return vectorRecord;
}

R_SurfaceRecord* S101Creator::ConvertInsertVectorRecord(SSurface* geom)
{
	auto vectorRecord = new R_SurfaceRecord();

	auto recordName = NewSurfaceRecordName();
	vectorRecord->m_srid = F_SRID(recordName);

	auto f_RIAS = new F_RIAS();

	for (auto i = geom->curveList.begin(); i != geom->curveList.end(); i++)
	{
		auto curveRecord = ConvertInsertVectorRecord(*i);
		
		auto rias = new RIAS();
		rias->m_name = curveRecord->GetRecordName();
		rias->m_ornt = 1;

		if (i == geom->curveList.begin())
		{
			rias->m_usag = 1;
		}
		else
		{
			rias->m_usag = 2;
		}
		
		rias->m_raui = 1;

		f_RIAS->m_arr.push_back(rias);
	}

	vectorRecord->m_rias.push_back(f_RIAS);

	enc->InsertRecord(vectorRecord);

	return vectorRecord;
}

std::list<AttributeBinding*> S101Creator::GetAddableAttributes(R_FeatureRecord* feature)
{
	std::list<AttributeBinding*> result;

	if (feature) 
	{
		auto featureCode = enc->m_dsgir.GetFeatureCode(feature->GetNumericCode());
		if (featureCode.IsEmpty() == false)
		{
			auto featureType = fc->GetFeatureType(std::wstring(featureCode));
			if (featureType)
			{
				result = featureType->GetAttributeBindingList();

				for (auto i = result.begin(); i != result.end(); )
				{
					auto currentAttributeBinding = *i;

					auto attributeNumericCode = enc->m_dsgir.GetAttributeCode(currentAttributeBinding->GetAttributeCodeAsWstring());

					auto currentRootAttribute = feature->GetRootAttributes(attributeNumericCode);

					if (
						currentAttributeBinding->GetMultiplicity().IsInfinite() == false &&
						currentAttributeBinding->GetMultiplicity().GetUpperCount() <= currentRootAttribute.size())
					{
						i = result.erase(i);
					}
					else
					{
						i++;
					}
				}
			}
		}
	}

	return result;
}

std::list<AttributeBinding*> S101Creator::GetAddableAttributes(R_FeatureRecord* feature, ATTR* parentATTR)
{
	std::list<AttributeBinding*> result;

	if (parentATTR)
	{
		auto attributeCode = enc->m_dsgir.GetAttributeCode(parentATTR->m_natc);
		if (attributeCode.IsEmpty() == false)
		{
			auto complexAttribute = fc->GetComplexAttribute(std::wstring(attributeCode));
			if (complexAttribute)
			{
				result = complexAttribute->GetAttributeBindingList();

				for (auto i = result.begin(); i != result.end(); )
				{
					auto currentAttributeBinding = *i;

					auto attributeNumericCode = enc->m_dsgir.GetAttributeCode(currentAttributeBinding->GetAttributeCodeAsWstring());

					auto currentChildAttributes = feature->GetChildAttributes(parentATTR, attributeNumericCode);

					if (
						currentAttributeBinding->GetMultiplicity().IsInfinite() == false &&
						currentAttributeBinding->GetMultiplicity().GetUpperCount() <= currentChildAttributes.size())
					{
						i = result.erase(i);
					}
					else
					{
						i++;
					}
				}
			}
		}
	}

	return result;
}

void S101Creator::AddDefaultChildAttributes(R_FeatureRecord* feature)
{
	auto featureCode = enc->m_dsgir.GetFeatureCode(feature->GetNumericCode());
	auto featureType = fc->GetFeatureType(std::wstring(featureCode));

	if (featureType)
	{
		auto attributeBindings = featureType->GetAttributeBindingList();
		for (auto i = attributeBindings.begin(); i != attributeBindings.end(); i++)
		{
			auto attributeBinding = *i;
			if (attributeBinding->GetLower() > 0)
			{
				auto attributeCode = attributeBinding->GetAttributeCodeAsWstring();

				auto simpleAttribute = fc->GetSimpleAttribute(attributeCode);
				if (simpleAttribute)
				{
					AddSimpleAttribute(feature, attributeCode, L"");
				}
				else
				{
					auto complexAttribute = fc->GetComplexAttribute(attributeCode);
					if (complexAttribute)
					{
						auto attr = AddComplexAttribute(feature, attributeCode);
						if (attr)
						{
							AddDefaultChildAttributes(feature, attr);
						}
					}
				}
			}
		}
	}
}

void S101Creator::AddDefaultChildAttributes(R_FeatureRecord* feature, ATTR* attr)
{
	auto attributeCode = enc->m_dsgir.GetAttributeCode(attr->m_natc);
	auto complexAttribute = fc->GetComplexAttribute(std::wstring(attributeCode));

	if (complexAttribute)
	{
		auto attributeBindings = complexAttribute->GetAttributeBindingList();
		for (auto i = attributeBindings.begin(); i != attributeBindings.end(); i++)
		{
			auto attributeBinding = *i;
			if (attributeBinding->GetLower() > 0)
			{
				auto attributeCode = attributeBinding->GetAttributeCodeAsWstring();

				auto simpleAttribute = fc->GetSimpleAttribute(attributeCode);
				if (simpleAttribute)
				{
					AddSimpleAttribute(feature, attr, attributeCode, L"");
				}
				else
				{
					auto complexAttribute = fc->GetComplexAttribute(attributeCode);
					if (complexAttribute)
					{
						auto newATTR = AddComplexAttribute(feature, attr, attributeCode);
						if (newATTR)
						{
							AddDefaultChildAttributes(feature, newATTR);
						}
					}
				}
			}
		}
	}
}