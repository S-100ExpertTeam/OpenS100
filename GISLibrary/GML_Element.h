#pragma once

#include <pugixml.hpp>

namespace S100GML
{
	class Element
	{
	public:
		virtual void Read(pugi::xml_node& node) { }
		virtual void Write(pugi::xml_node& node) { }
	};
}