#pragma once
#include "SpatialObject.h"

#include "..\\GeoMetryLibrary\\MBR.h"

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
	void SetMBR(MBR& value);
	MBR GetMBR();
	void SetPCOutputManager(PCOutputSchemaManager* value);
	PCOutputSchemaManager* GetPCOutputManager();
};