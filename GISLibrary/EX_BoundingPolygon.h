#pragma once
#include "GM_Object_New.h"
#include <pugixml.hpp>


namespace S100 {

	class EX_BoundingPolygon
	{
	public:
		bool ExtentTypeCode;
		GM_Object Polygon;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();
				
				if (!strcmp(instructionName, "gex:extentTypeCode"))
				{
					ExtentTypeCode = ParseStr2Bool(instruction.child_value());
				}
				else if (!strcmp(instructionName, "gex:polygon"))
				{
					GM_Object go;
					go.GetContents(node);
					Polygon = go;
				}
				else
				{
				}
			}
		}
	};
}

