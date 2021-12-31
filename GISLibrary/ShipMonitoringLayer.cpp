#include "stdafx.h"
#include "ShipMonitoringLayer.h"
#include "ShipMonitoringXMLFile.h"

ShipMonitoringLayer::ShipMonitoringLayer()
{

}

ShipMonitoringLayer::~ShipMonitoringLayer()
{

}

bool ShipMonitoringLayer::Open(CString _filePath)
{
	m_spatialObject = new ShipMonitoringXMLFile();
	m_spatialObject->m_pLayer = this;
	return m_spatialObject->Open(_filePath);
}
