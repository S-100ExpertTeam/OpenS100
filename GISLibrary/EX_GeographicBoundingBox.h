#pragma once
#include <string>
#include "pugixml.hpp"
#include "pugiSupportClass.h"

namespace S100 {

	class EX_GeographicBoundingBox {
	public:
		double WestBoundLongitude;
		double EastBoundLongitude;
		double SouthBoundLatitude;
		double NorthBoundLatitude;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "gex:westBoundLongitude"))
				{
					WestBoundLongitude = atof(std::string(GetContentItem(instruction, "gco:Decimal")).c_str());
				}
				else if (!strcmp(instructionName, "gex:eastBoundLongitude"))
				{
					EastBoundLongitude = atof(std::string(GetContentItem(instruction, "gco:Decimal")).c_str());
				}
				else if (!strcmp(instructionName, "gex:southBoundLatitude"))
				{
					SouthBoundLatitude = atof(std::string(GetContentItem(instruction, "gco:Decimal")).c_str());
				}
				else if (!strcmp(instructionName, "gex:northBoundLatitude"))
				{
					NorthBoundLatitude = atof(std::string(GetContentItem(instruction, "gco:Decimal")).c_str());
				}
			}
		}
	};
}
