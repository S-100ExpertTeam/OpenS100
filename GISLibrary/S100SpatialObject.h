#pragma once

#include "SpatialObject.h"

#include "../GeoMetryLibrary/MBR.h"

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
	void SetPCOutputManager(PCOutputSchemaManager* value);
	PCOutputSchemaManager* GetPCOutputManager();

	virtual std::wstring GetFeatureTypeCodeByID(std::wstring id) { return L""; }
	virtual std::wstring GetFeatureTypeCodeByID(int id) { return L""; }
};