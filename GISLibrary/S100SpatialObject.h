#pragma once

#include "S100_GF.h"
#include "SpatialObject.h"

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
	S100SpatialObject();
	virtual ~S100SpatialObject();

public:
	PCOutputSchemaManager* pcManager = nullptr;

public:
	void InitDrawingInstruction();
	PCOutputSchemaManager* GetPCOutputManager();

	int GetProductNumber();

	FeatureCatalogue* GetFC();
	PortrayalCatalogue* GetPC();

public:
	// type = 1 : feature type
	// type = 2 : information type
	virtual GF::ObjectType* GetObjectType(int type, std::string id) { return nullptr; }

	virtual std::wstring GetFeatureTypeCodeByID(std::wstring id) { return L""; }
	virtual std::wstring GetFeatureTypeCodeByID(int id) { return L""; }

	virtual std::wstring GetInformationTypeCodeByID(std::wstring id) { return L""; }
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

	virtual std::string GetObjectAttributeCode(int type, std::string, int index) { return ""; }

	virtual int GetFeatureAttributeCount(std::string id) { return 0; }
	virtual std::string GetFeatureAttributeCode(std::string, int index) { return ""; }

	virtual int GetInformationAttributeCount(std::string id) { return 0; }
	virtual std::string GetInformationAttributeCode(std::string, int index) { return ""; }

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