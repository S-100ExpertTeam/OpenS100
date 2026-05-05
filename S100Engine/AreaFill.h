#pragma once

#include <pugixml.hpp>

#include <vector>
class AreaFill
{
public:
	AreaFill();
	virtual ~AreaFill();

public:
	// S100Meta::name
	std::string _name;

	// S100Meta::exposition
	std::string _exposition;

	// areaCRS
	std::string _areaCRS;

	// symbol::reference
	std::string _symbolReference;

	// symbol::v1,v2
	float v1_x;
	float v1_y;
	float v2_x;
	float v2_y;

	// Symboll::offset
	float offset_x = 0;
	float offset_y = 0;

public:

	bool Read(const std::string& path);
	bool Read(const std::wstring& path);
	bool ReadByPugi(const std::string& path);
	bool ReadByPugi(const std::wstring& path);
	bool initializeCOM();
	std::string getNodeValue(pugi::xml_node node);
	std::string getNodeAttribute(pugi::xml_node node);
};