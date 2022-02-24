#pragma once

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <vector>
class AreaFill
{
public:
	AreaFill();
	virtual ~AreaFill();

public:
	// ?S100Meta::name
	std::wstring _name;

	// ?S100Meta::exposition
	std::wstring _exposition;

	// areaCRS
	std::wstring _areaCRS;

	// symbol::reference
	std::wstring _symbolReference;

	// symbol::v1,v2
	float v1_x;
	float v1_y;
	float v2_x;
	float v2_y;

public:

	bool Read(std::wstring path);
	bool ReadByPugi(std::wstring path);
	bool initializeCOM();
	//std::wstring getNodeValue(IXMLDOMDocumentPtr &pDoc, BSTR selectSingleNode);
	std::wstring getNodeValue(pugi::xml_node node);
	/*std::wstring getNodeAttribute(IXMLDOMDocumentPtr &pDoc, BSTR selectSingleNode);*/
	std::wstring getNodeAttribute(pugi::xml_node node);
};