#include "stdafx.h"
#include "S101Factory.h"

#include "S100Layer.h"

#include "S102_RootGroup.h"

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

SHPFile* S100LayerFactory::CopySHPFile(Layer* layer, CString newName)
{
	if (!layer)
		return nullptr;

	SHPFile* enc = (SHPFile*)layer->GetSpatialObject();
	if (!enc)
		return nullptr;

	SHPFile* shpFile = new SHPFile();
	shpFile->SetFilePath(newName);
	shpFile->m_pLayer = layer;

	shpFile->m_pLayer->m_mbr.xmin = enc->m_pLayer->m_mbr.xmin;
	shpFile->m_pLayer->m_mbr.ymin = enc->m_pLayer->m_mbr.ymin;
	shpFile->m_pLayer->m_mbr.xmax = enc->m_pLayer->m_mbr.xmax;
	shpFile->m_pLayer->m_mbr.ymax = enc->m_pLayer->m_mbr.ymax;

	shpFile->m_nRecords = enc->m_nRecords;
	shpFile->m_pSHPObject = new Geometry * [shpFile->m_nRecords];

	switch (enc->m_nShapeType)
	{
	case GEOPOINT:
	{
		for (unsigned int i = 0; i < enc->m_nRecords; i++)
		{
			GeoPoint* gp = (GeoPoint*)enc->m_pSHPObject[i];
			shpFile->m_pSHPObject[i] = new GeoPoint(*gp);
		}
	}
	break;
	case GEOPOLYLINE:
	{
		for (unsigned int i = 0; i < enc->m_nRecords; i++)
		{
			GeoPolyline* gl = (GeoPolyline*)enc->m_pSHPObject[i];
			shpFile->m_pSHPObject[i] = new GeoPolyline(*gl);
		}
	}
	break;
	case GEOPOLYGON:
	{
		for (unsigned int i = 0; i < enc->m_nRecords; i++)
		{
			GeoPolyline* gp = (GeoPolyline*)enc->m_pSHPObject[i];
			shpFile->m_pSHPObject[i] = new GeoPolyline(*gp);
		}
	}
	break;
	default:
		break;
	}

	return shpFile;
}

S101Cell* S100LayerFactory::CopyS101Cell(Layer* layer, CString newName)
{
	if (!layer)
		return nullptr;

	S101Cell* enc = (S101Cell*)layer->GetSpatialObject();
	if (!enc)
		return nullptr;

	S101Cell* cell = new S101Cell(enc->GetD2());
	cell->SetFilePath(newName);
	cell->m_pLayer = layer;

	cell->m_dsgir = enc->m_dsgir;
	cell->m_dscrs = enc->m_dscrs;

	for (const auto& iter : enc->GetVecInformation())
	{
		R_InformationRecord* ir = new R_InformationRecord(*iter);
		cell->InsertInformationRecord(ir->m_irid.m_name.GetName(), ir);
	}

	for (const auto& iter : enc->GetVecPoint())
	{
		R_PointRecord* pr = new R_PointRecord(*iter);
		cell->InsertPointRecord(pr->m_prid.m_name.GetName(), pr);
	}

	for (const auto& iter : enc->GetVecMultiPoint())
	{
		R_MultiPointRecord* mpr = new R_MultiPointRecord(*iter);
		cell->InsertMultiPointRecord(mpr->m_mrid.m_name.GetName(), mpr);
	}

	for (const auto& iter : enc->GetVecCurve())
	{
		R_CurveRecord* cr = new R_CurveRecord(*iter);
		cell->InsertCurveRecord(cr->m_crid.m_name.GetName(), cr);
	}

	for (const auto& iter : enc->GetVecComposite())
	{
		R_CompositeRecord* ccr = new R_CompositeRecord(*iter);
		cell->InsertCompositeCurveRecord(ccr->m_ccid.m_name.GetName(), ccr);
	}

	for (const auto& iter : enc->GetVecSurface())
	{
		R_SurfaceRecord* sr = new R_SurfaceRecord(*iter);
		cell->InsertSurfaceRecord(sr->m_srid.m_name.GetName(), sr);
	}

	for (const auto& iter : enc->GetVecFeature())
	{
		R_FeatureRecord* fr = new R_FeatureRecord(*iter);
		cell->InsertFeatureRecord(fr->m_frid.m_name.GetName(), fr);
	}

	cell->MakeFullSpatialData();
	cell->CalcMBR();
	cell->Check();

	cell->ATTRtoAttribute();

	return cell;
}

S10XGML* S100LayerFactory::CopyS10XGML(Layer* layer, CString newName)
{
	if (!layer)
		return nullptr;

	S10XGML* enc = (S10XGML*)layer->GetSpatialObject();
	if (!enc)
		return nullptr;

	S10XGML* gml = new S10XGML(enc->GetD2());
	gml->SetFilePath(newName);
	gml->m_pLayer = layer;

	gml->envelop.mbr = enc->envelop.mbr;
	gml->datasetIdentificationInformation = enc->datasetIdentificationInformation;

	for (const auto& iter : enc->features)
	{
		GF::FeatureType* ft = new GF::FeatureType(*iter);
		gml->features.push_back(ft);
	}

	for (const auto& iter : enc->informations)
	{
		GF::InformationType* it = new GF::InformationType(*iter);
		gml->informations.push_back(it);
	}

	for (const auto& iter : enc->geometries)
	{
		GM::Object* o = new GM::Object(*iter);
		gml->geometries.push_back(o);
	}

	gml->SetGeometry();
	gml->CalcMBR();

	return gml;
}

S100H5* S100LayerFactory::CopyS100H5(Layer* layer, CString newName)
{
	if (!layer)
		return nullptr;

	S100H5* enc = (S100H5*)layer->GetSpatialObject();
	if (!enc)
		return nullptr;

	S100H5* h5 = new S100H5(enc->GetD2());
	h5->SetFilePath(newName);
	h5->m_pLayer = layer;

	h5->rootGroup = new H5_RootGroup(*enc->rootGroup);
	h5->SetMBR();

	return h5;
}

S102H5* S100LayerFactory::CopyS102H5(Layer* layer, CString newName)
{
	if (!layer)
		return nullptr;

	S102H5* enc = (S102H5*)layer->GetSpatialObject();
	if (!enc)
		return nullptr;

	S102H5* h5 = new S102H5(enc->GetPC(), enc->GetD2());

	h5->SetFilePath(newName);
	h5->m_pLayer = layer;

	S102_RootGroup* roopGroup = (S102_RootGroup*)enc->rootGroup;
	h5->rootGroup = new S102_RootGroup(*roopGroup);

	for (const auto& iter : enc->featureContainer)
	{
		H5_FeatureContainer* container = new H5_FeatureContainer(*iter);
		h5->featureContainer.push_back(container);
	}

	h5->SetMBR();

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
		SHPFile* dscObject = S100LayerFactory::CopySHPFile(srcLayer, newName);
		if (dscObject)
		{
			dscLayer = new Layer();
			dscLayer->m_spatialObject = dscObject;
		}
	}
	else if (fileType == S100_FileType::FILE_S_100_VECTOR)
	{
		S100SpatialObjectType objType = ((S100SpatialObject*)srcLayer->m_spatialObject)->type;
		if (objType == S100SpatialObjectType::S101Cell)
		{
			S101Cell* dscObject = S100LayerFactory::CopyS101Cell(srcLayer, newName);
			if (dscObject)
			{
				auto fc = ((S100Layer*)srcLayer)->GetFC();
				auto pc = ((S100Layer*)srcLayer)->GetPC();
				dscLayer = new S100Layer(fc, pc);
				dscLayer->m_spatialObject = dscObject;

				((S100Layer*)dscLayer)->BuildPortrayalCatalogue();
			}
		}
		else if (objType == S100SpatialObjectType::S10XGML)
		{
			S10XGML* dscObject = S100LayerFactory::CopyS10XGML(srcLayer, newName);
			if (dscObject)
			{
				auto fc = ((S100Layer*)srcLayer)->GetFC();
				auto pc = ((S100Layer*)srcLayer)->GetPC();
				dscLayer = new S100Layer(fc, pc);
				dscLayer->m_spatialObject = dscObject;

				((S100Layer*)dscLayer)->BuildPortrayalCatalogue();
			}
		}
		else if (objType == S100SpatialObjectType::S100H5)
		{
			S100H5* dscObject = S100LayerFactory::CopyS100H5(srcLayer, newName);
		}
		else if (objType == S100SpatialObjectType::S102H5)
		{
			S102H5* dscObject = S100LayerFactory::CopyS102H5(srcLayer, newName);
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

