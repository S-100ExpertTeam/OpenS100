#include "stdafx.h"
#include "BAGLayer.h"
#include "BAG.h"

BAGLayer::BAGLayer()
{
	m_spatialObject = new BAG();
	m_spatialObject->m_pLayer = this;

	m_pFeatureCatalogue = nullptr;
	m_pPortrayalCatalogue = nullptr;
}

BAGLayer::BAGLayer(CString _path)
{
	m_spatialObject = new BAG();
	m_spatialObject->m_pLayer = this;

	m_pFeatureCatalogue = nullptr;
	m_pPortrayalCatalogue = nullptr;
}

BAGLayer::~BAGLayer()
{
}

bool BAGLayer::Open(CString _filepath)
{
	m_spatialObject = new BAG();
	m_spatialObject->m_pLayer = this;

	if (!m_spatialObject->Open(_filepath))
	{
		return false;
	}
	if (!BuildInputXML())
	{
		return false;
	}
	return true;
}

void BAGLayer::UpdateS102ColorMapType(int _type, std::vector<COLORREF> _colorMap)
{
	((BAG*)m_spatialObject)->bBMP = false;
	((BAG*)m_spatialObject)->bmpGrid.DeleteObject();
	((BAG*)m_spatialObject)->bmpGridback.DeleteObject();
	((BAG*)m_spatialObject)->bmptmpGrid.DeleteObject();
	((BAG*)m_spatialObject)->colorMap = _colorMap;
	((BAG*)m_spatialObject)->dc.DeleteDC();
	((BAG*)m_spatialObject)->tmpdc.DeleteDC();
	((BAG*)m_spatialObject)->dcback.DeleteDC();
	((BAG*)m_spatialObject)->s102ColorMapType = _type;
}

void BAGLayer::GetS102ColorMapType(int& _type, std::vector<COLORREF>& _colorMap)
{
	_type = ((BAG*)m_spatialObject)->s102ColorMapType;
	_colorMap = ((BAG*)m_spatialObject)->colorMap;
}

bool BAGLayer::BuildInputXML()
{
	if (m_spatialObject)
	{
		return true;
	}
	return false;
}
