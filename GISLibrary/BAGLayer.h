#pragma once
#include "Layer.h"
#include <vector>

class PortrayalCatalogue;
class Catalog;

class BAGLayer :
	public Layer
{
public:
	BAGLayer();
	BAGLayer(CString _path);

public:
	std::vector<COLORREF> s102colorMap;
	int s102ColorMapType;
	PortrayalCatalogue		*m_pPortrayalCatalogue;
	Catalog					*m_pFeatureCatalogue;

public:
	virtual bool Open(CString _filepath);
	virtual ~BAGLayer();
	//void UpdateS102ColorMapType(int _type, std::vector<COLORREF> _colorMap);
	//void GetS102ColorMapType(int& _type, std::vector<COLORREF>& _colorMap);
	bool BuildInputXML();

};