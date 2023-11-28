#include "stdafx.h"
#include "S101Creator.h"

#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCurve.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "SCoverage.h"

#include "R_PointRecord.h"
#include "R_MultiPointRecord.h"
#include "R_CurveRecord.h"
#include "R_CompositeRecord.h"
#include "R_SurfaceRecord.h"
#include "R_FeatureRecord.h"
#include "R_InformationRecord.h"

SGeometry* S101GeometryFactory::createFromWkb(void* pabyData, size_t nBytes)
{
	unsigned char* l_pabyData = static_cast<unsigned char*>(pabyData);
	SGeometry* ppoReturn = nullptr;

	if (nBytes < 9 && nBytes != static_cast<size_t>(-1))
		return ppoReturn;

	const int nByteOrder = DB2_V72_FIX_BYTE_ORDER(*l_pabyData);
	if (nByteOrder != wkbXDR && nByteOrder != wkbNDR)
		return ppoReturn;

	SGeometryType eGeometryType = S101GeometryUtil::ReadWKBGeometryType(l_pabyData);
	if (eGeometryType == SGeometryType::none)
		return ppoReturn;

	SGeometry* poGeom = createGeometry(eGeometryType);
	if (poGeom == nullptr)
		return ppoReturn;

	poGeom->ImportFromWkb(l_pabyData, nBytes);

	return ppoReturn;
}

SGeometry* S101GeometryFactory::createGeometry(SGeometryType eGeometryType)
{
	SGeometry* poGeom = nullptr;
	switch (eGeometryType)
	{
	case SGeometryType::Point:
		poGeom = new (std::nothrow) SPoint();
		break;
	case SGeometryType::MultiPoint:
		poGeom = new (std::nothrow) SMultiPoint();
		break;
	case SGeometryType::Curve:
		poGeom = new (std::nothrow) SCurve();
		break;
	case SGeometryType::CompositeCurve:
		poGeom = new (std::nothrow) SCompositeCurve();
		break;
	case SGeometryType::Surface:
		poGeom = new (std::nothrow) SSurface();
		break;
	case SGeometryType::Coverage:
		poGeom = new (std::nothrow) SCoverage();
		break;
	default:
		break;
	}

	return poGeom;
}

void S101GeometryFactory::destroyGeometry(SGeometry* poGeom)
{
	if (poGeom)
	{
		delete poGeom;
		poGeom = nullptr;
	}
}

Record* S101GeometryFactory::createRecord(GISLibrary::RCNM eRCNM)
{
	Record* poRecord = nullptr;
	switch (eRCNM)
	{
	case GISLibrary::RCNM::Point:
		poRecord = new (std::nothrow) R_PointRecord();
		break;
	case GISLibrary::RCNM::MultiPoint:
		poRecord = new (std::nothrow) R_MultiPointRecord();
		break;
	case GISLibrary::RCNM::Curve:
		poRecord = new (std::nothrow) R_CurveRecord();
		break;
	case GISLibrary::RCNM::CompositeCurve:
		poRecord = new (std::nothrow) R_CompositeRecord();
		break;
	case GISLibrary::RCNM::Surface:
		poRecord = new (std::nothrow) R_SurfaceRecord();
		break;
	case GISLibrary::RCNM::FeatureType:
		poRecord = new (std::nothrow) R_FeatureRecord();
		break;
	case GISLibrary::RCNM::InformationType:
		poRecord = new (std::nothrow) R_InformationRecord();
		break;
	default:
		break;
	}

	return poRecord;
}

void S101GeometryFactory::destroyRecord(Record* poRecord)
{
	if (poRecord)
	{
		delete poRecord;
		poRecord = nullptr;
	}
}