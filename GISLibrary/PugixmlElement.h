#pragma once

#include <pugixml.hpp>

class PugixmlElement
{
public:
	virtual void Read(pugi::xml_node& node) { }
	virtual void Write(pugi::xml_node& node) { }
};
