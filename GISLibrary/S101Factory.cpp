#include "stdafx.h"
#include "S101Factory.h"

#include "S100Layer.h"

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

#include "F_PTAS.h"
#include "F_SEGH.h"
#include "F_CUCO.h"
#include "F_RIAS.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "F_SPAS.h"
#include "F_FASC.h"
#include "F_MASK.h"

SGeometry* S101GeometryFactory::CreateFromWkb(void* pabyData, size_t nBytes)
{
	unsigned char* l_pabyData = static_cast<unsigned char*>(pabyData);
	SGeometry* ppoReturn = nullptr;

	if (nBytes < 9 && nBytes != static_cast<size_t>(-1))
		return ppoReturn;

	const int nByteOrder = DB2_V72_FIX_BYTE_ORDER(*l_pabyData);
	if (nByteOrder != XDR && nByteOrder != NDR)
		return ppoReturn;

	SGeometryType eGeometryType = S101GeometryUtil::ReadWKBGeometryType(l_pabyData);
	if (eGeometryType == SGeometryType::none)
		return ppoReturn;

	SGeometry* poGeom = CreateGeometry(eGeometryType);
	if (poGeom == nullptr)
		return ppoReturn;

	poGeom->ImportFromWkb(l_pabyData, (int)nBytes);

	return ppoReturn;
}

SGeometry* S101GeometryFactory::CreateGeometry(SGeometryType eGeometryType)
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

void S101GeometryFactory::DestroyGeometry(SGeometry* poGeom)
{
	if (poGeom)
	{
		delete poGeom;
		poGeom = nullptr;
	}
}

Record* S101GeometryFactory::CreateRecord(RCNM eRCNM)
{
	Record* poRecord = nullptr;
	switch (eRCNM)
	{
	case RCNM::Point:
		poRecord = new (std::nothrow) R_PointRecord();
		break;
	case RCNM::MultiPoint:
		poRecord = new (std::nothrow) R_MultiPointRecord();
		break;
	case RCNM::Curve:
		poRecord = new (std::nothrow) R_CurveRecord();
		break;
	case RCNM::CompositeCurve:
		poRecord = new (std::nothrow) R_CompositeRecord();
		break;
	case RCNM::Surface:
		poRecord = new (std::nothrow) R_SurfaceRecord();
		break;
	case RCNM::FeatureType:
		poRecord = new (std::nothrow) R_FeatureRecord();
		break;
	case RCNM::InformationType:
		poRecord = new (std::nothrow) R_InformationRecord();
		break;
	default:
		break;
	}

	return poRecord;
}

void S101GeometryFactory::DestroyRecord(Record* poRecord)
{
	if (poRecord)
	{
		delete poRecord;
		poRecord = nullptr;
	}
}

Field* S101FieldFactory::CreateField(S101FieldType FieldType)
{
	Field* poField = nullptr;

	switch (FieldType)
	{
	case Field_DSID:
		poField = new F_DSID();
		break;
	case Field_PRID:
		poField = new F_PRID();
		break;
	case Field_MRID:
		poField = new F_MRID();
		break;
	case Field_CRID:
		poField = new F_CRID();
		break;
	case Field_CCID:
		poField = new F_CCID();
		break;
	case Field_SRID:
		poField = new F_SRID();
		break;
	case Field_FRID:
		poField = new F_FRID();
		break;
	case Field_PTAS:
		poField = new F_PTAS();
		break;
	case Field_SEGH:
		poField = new F_SEGH();
		break;
	case Field_CUCO:
		poField = new F_CUCO();
		break;
	case Field_RIAS:
		poField = new F_RIAS();
		break;
	case Field_ATTR:
		poField = new F_ATTR();
		break;
	case Field_INAS:
		poField = new F_INAS();
		break;
	case Field_SPAS:
		poField = new F_SPAS();
		break;
	case Field_FASC:
		poField = new F_FASC();
		break;
	case Field_MASK:
		poField = new F_MASK();
		break;
	default:
		break;

	}

	return poField;
}

void S101FieldFactory::DestoryField(Field* poField)
{
	if (poField)
	{
		delete poField;
		poField = nullptr;
	}
}

SGeometryType S101GeometryUtil::ReadWKBGeometryType(const unsigned char* pabyData)
{
	int nByteOrder = DB2_V72_FIX_BYTE_ORDER(*pabyData);
	if (!(nByteOrder == XDR || nByteOrder == NDR))
		return SGeometryType::none;

	int iRawType = 0;
	memcpy(&iRawType, pabyData + 1, 4);

	if (iRawType == 2000001)
		return SGeometryType::Point;
	else if (iRawType == 2000002)
		return SGeometryType::Curve;
	else if (iRawType == 2000005)
		return SGeometryType::Surface;
	else if (iRawType == 2000007)
		return SGeometryType::MultiPoint;
	else if (iRawType == 3000001)
		return SGeometryType::Point;
	else if (iRawType == 3000002)
		return SGeometryType::Curve;
	else if (iRawType == 3000005)
		return SGeometryType::Surface;
	else if (iRawType == 3000007)
		return SGeometryType::MultiPoint;
	else if (iRawType == 4000001)
		return SGeometryType::Point;
	else if (iRawType == 4000002)
		return SGeometryType::Curve;
	else if (iRawType == 4000005)
		return SGeometryType::Surface;
	else if (iRawType == 4000007)
		return SGeometryType::MultiPoint;

	return SGeometryType::none;
}

SHPFile* S100LayerFactory::CopySHPFile(SHPFile* enc, CString newName)
{
	if (!enc)
		return nullptr;

	SHPFile* shpFile = new SHPFile();
	shpFile->SetFilePath(newName);

	shpFile->m_pLayer->m_mbr.xmin = enc->m_pLayer->m_mbr.xmin;
	shpFile->m_pLayer->m_mbr.ymin = enc->m_pLayer->m_mbr.ymin;
	shpFile->m_pLayer->m_mbr.xmax = enc->m_pLayer->m_mbr.xmax;
	shpFile->m_pLayer->m_mbr.ymax = enc->m_pLayer->m_mbr.ymax;

	shpFile->m_nRecords = enc->m_nRecords;
	shpFile->m_pSHPObject = new Geometry * [shpFile->m_nRecords];
	for (unsigned int i = 0; i < enc->m_nRecords; i++)
		shpFile->m_pSHPObject[i] = enc->m_pSHPObject[i]->Clone();

	return shpFile;
}

S101Cell* S100LayerFactory::CopyS101Cell(S101Cell* enc, CString newName)
{
	if (!enc)
		return nullptr;

	S101Cell* cell = new S101Cell(enc->GetD2());
	cell->SetFilePath(newName);

	cell->m_dsgir = enc->m_dsgir;
	cell->m_dscrs = enc->m_dscrs;

	for (const auto& iter : enc->GetVecInformation())
	{
		cell->InsertInformationRecord(iter->m_irid.m_name.GetName(), iter);
	}

	for (const auto& iter : enc->GetVecPoint())
	{
		cell->InsertPointRecord(iter->m_prid.m_name.GetName(), iter);
	}

	for (const auto& iter : enc->GetVecMultiPoint())
	{
		cell->InsertMultiPointRecord(iter->m_mrid.m_name.GetName(), iter);
	}

	for (const auto& iter : enc->GetVecCurve())
	{
		cell->InsertCurveRecord(iter->m_crid.m_name.GetName(), iter);
	}

	for (const auto& iter : enc->GetVecComposite())
	{
		cell->InsertCompositeCurveRecord(iter->m_ccid.m_name.GetName(), iter);
	}

	for (const auto& iter : enc->GetVecSurface())
	{
		cell->InsertSurfaceRecord(iter->m_srid.m_name.GetName(), iter);
	}

	for (const auto& iter : enc->GetVecFeature())
	{
		cell->InsertFeatureRecord(iter->m_frid.m_name.GetName(), iter);
	}

	cell->MakeFullSpatialData();
	cell->CalcMBR();
	cell->Check();

	cell->ATTRtoAttribute();

	return cell;
}

S10XGML* S100LayerFactory::CopyS10XGML(S10XGML* enc, CString newName)
{
	if (!enc)
		return nullptr;

	S10XGML* gml = new S10XGML(enc->GetD2());
	gml->SetFilePath(newName);

	gml->envelop.mbr = enc->envelop.mbr;
	gml->datasetIdentificationInformation = enc->datasetIdentificationInformation;

	for (const auto& iter : enc->features)
	{
		GF::FeatureType* ft = iter->Clone();
		gml->features.push_back(ft);
	}

	for (const auto& iter : enc->informations)
	{
		GF::InformationType* it = iter->Clone();
		gml->informations.push_back(it);
	}

	for (const auto& iter : enc->geometries)
	{
		GM::Object* o = iter->Clone();
		gml->geometries.push_back(o);
	}

	gml->SetGeometry();
	gml->CalcMBR();

	return gml;
}

S100H5* S100LayerFactory::CopyS100H5(S100H5* enc, CString newName)
{
	if (!enc)
		return nullptr;

	return nullptr;
}

S102H5* S100LayerFactory::CopyS102H5(S102H5* enc, CString newName)
{
	if (!enc)
		return nullptr;

	return nullptr;
}

Layer* S100LayerFactory::CopyLayer(Layer* srcLayer, CString newName)
{
	Layer* dscLayer = nullptr;

	if ((!srcLayer) ||
		(!srcLayer->m_spatialObject))
		return dscLayer;

	S100_FileType fileType = srcLayer->m_spatialObject->m_FileType;

	if (fileType == S100_FileType::FILE_Shape)
	{
		SHPFile* dscObject = S100LayerFactory::CopySHPFile((SHPFile*)srcLayer->m_spatialObject, newName);
		if (dscObject)
		{
			dscLayer = new Layer();
			dscLayer->m_spatialObject = dscObject;
			dscObject->m_pLayer = dscLayer;
		}
	}
	else if (fileType == S100_FileType::FILE_S_100_VECTOR)
	{
		S100SpatialObjectType objType = ((S100SpatialObject*)srcLayer->m_spatialObject)->type;
		if (objType == S100SpatialObjectType::S101Cell)
		{
			S101Cell* dscObject = S100LayerFactory::CopyS101Cell((S101Cell*)srcLayer->m_spatialObject, newName);
			if (dscObject)
			{
				auto fc = ((S100Layer*)srcLayer)->GetFC();
				auto pc = ((S100Layer*)srcLayer)->GetPC();
				dscLayer = new S100Layer(fc, pc);
				dscLayer->m_spatialObject = dscObject;
				dscObject->m_pLayer = dscLayer;

				((S100Layer*)dscLayer)->BuildPortrayalCatalogue();
			}
		}
		else if (objType == S100SpatialObjectType::S10XGML)
		{
			S10XGML* dscObject = S100LayerFactory::CopyS10XGML((S10XGML*)srcLayer->m_spatialObject, newName);
			if (dscObject)
			{
				auto fc = ((S100Layer*)srcLayer)->GetFC();
				auto pc = ((S100Layer*)srcLayer)->GetPC();
				dscLayer = new S100Layer(fc, pc);
				dscLayer->m_spatialObject = dscObject;
				dscObject->m_pLayer = dscLayer;

				((S100Layer*)dscLayer)->BuildPortrayalCatalogue();
			}
		}
		else if (objType == S100SpatialObjectType::S100H5)
		{

		}
		else if (objType == S100SpatialObjectType::S102H5)
		{

		}
	}

	return dscLayer;
}

void S100LayerFactory::DestoryLayer(Layer* pLayer)
{
	if (pLayer)
	{
		delete pLayer;
		pLayer = nullptr;
	}
}

