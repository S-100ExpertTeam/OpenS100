#pragma once
#include "XML_Item.h"
#include "Citation.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class DefinitionSource :
	public XML_Item
{
public:
	DefinitionSource();
	virtual	~DefinitionSource();
	
private:
	Citation source;
	
public:
	void GetContents(pugi::xml_node& node);

	void SetSource(Citation value);
	Citation& GetSource();
};