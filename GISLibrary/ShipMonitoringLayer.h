#pragma once
#include "Layer.h"

class ShipMonitoringLayer :
	public Layer
{
public:
	ShipMonitoringLayer();
	virtual ~ShipMonitoringLayer();

public:
	bool Open(CString _filePath);
};