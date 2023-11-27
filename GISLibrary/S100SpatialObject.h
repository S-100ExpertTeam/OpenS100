#pragma once

#include "S100_GF.h"
#include "SpatialObject.h"
#include "S100SpatialObjectType.h"

#include "../GeoMetryLibrary/MBR.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <vector>
#include <string>

class PCOutputSchemaManager;
class S100Layer;

class S100SpatialObject : public SpatialObject
{
public:
	S100SpatialObject(GISLibrary::D2D1Resources* d2d1);
	virtual ~S100SpatialObject();

public:
	S100SpatialObjectType type = S100SpatialObjectType::S100SpatialObject;
	PCOutputSchemaManager* pcManager = nullptr;
	GISLibrary::D2D1Resources* D2 = nullptr;

public:
	S100SpatialObjectType getS100SpatialObjectType();

	void InitDrawingInstruction();
	PCOutputSchemaManager* GetPCOutputManager();

	int GetProductNumber();

	FeatureCatalogue* GetFC();
	PortrayalCatalogue* GetPC();

	bool OpenOutputXML(std::string path);

public:
	// type = 1 : feature type
	// type = 2 : information type
	virtual GF::ObjectType* GetObjectType(int type, std::string id);

	virtual std::wstring GetFeatureTypeCodeByID(std::wstring id) { return L""; }
	virtual std::wstring GetFeatureTypeCodeByID(std::string id) { return L""; }
	virtual std::wstring GetFeatureTypeCodeByID(int id) { return L""; }

	virtual std::wstring GetInformationTypeCodeByID(std::wstring id) { return L""; }
	virtual std::wstring GetInformationTypeCodeByID(std::string id) { return L""; }
	virtual std::wstring GetInformationTypeCodeByID(int id) { return L""; }

	virtual int GetFeatureCount() { return 0; }
	virtual int GetInformationCount() { return 0; }

	virtual GF::FeatureType* GetFeatureType(std::string id) { return nullptr; }
	virtual GF::FeatureType* GetFeatureTypeByIndex(int index) { return nullptr; }

	virtual GF::InformationType* GetInformationType(std::string id) { return nullptr; }
	virtual GF::InformationType* GetInformationTypeByIndex(int index) { return nullptr; }

	virtual std::string GetFeatureAssociationCode(GF::FeatureType* featureType, int index) { return ""; }
	virtual std::string GetFeatureAssociationRoleCode(GF::FeatureType* featureType, int index) { return ""; }

	virtual std::string GetInformationAssociationCode(GF::FeatureType* featureType, int index) { return ""; }
	virtual std::string GetInformationAssociationRoleCode(GF::FeatureType* featureType, int index) { return ""; }

	virtual std::string GetInformationAssociationCode(GF::InformationType* informationType, int index) { return ""; }
	virtual std::string GetInformationAssociationRoleCode(GF::InformationType* informationType, int index) { return ""; }

	virtual std::string GetObjectAttributeCode(int type, std::string id, int index);

	virtual int GetFeatureAttributeCount(std::string id);
	virtual std::string GetFeatureAttributeCode(std::string id, int index);

	virtual int GetInformationAttributeCount(std::string id);
	virtual std::string GetInformationAttributeCode(std::string id, int index);

	virtual GM::Object* GetGeometry(std::string id) { return nullptr; }

	virtual int CoordinateMultiplicationFactorForX() { return 0; }
	virtual int CoordinateMultiplicationFactorForY() { return 0; }
	virtual int CoordinateMultiplicationFactorForZ() { return 0; }

	virtual bool SaveToInputXML(std::string path) { return false; };

	// input unit : WGS84
	virtual std::vector<std::string> Query(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToSurface(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToCurve(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToPoint(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToMultiPoint(MBR mbr) { return {}; };
};