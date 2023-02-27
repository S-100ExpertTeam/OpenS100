#pragma once

#include "SpatialObject.h"

#include "../GeoMetryLibrary/MBR.h"

#include <vector>
#include <string>

class PCOutputSchemaManager;
class S100Layer;

namespace IF
{
	class FeatureType;
}

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

	virtual std::wstring GetFeatureTypeCodeByID(std::wstring id) { return L""; }
	virtual std::wstring GetFeatureTypeCodeByID(int id) { return L""; }

	virtual int GetFeatureCount() { return 0; }

	virtual IF::FeatureType* GetFeature(std::string id) { return nullptr; };
	virtual IF::FeatureType* GetFeatureByIndex(int index) { return nullptr; };

	// input unit : WGS84
	virtual std::vector<std::string> Query(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToSurface(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToCurve(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToPoint(MBR mbr) { return {}; };
	virtual std::vector<std::string> QueryToMultiPoint(MBR mbr) { return {}; };
};