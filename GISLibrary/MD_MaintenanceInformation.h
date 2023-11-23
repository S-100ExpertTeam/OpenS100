#pragma once
#include "CI_Date.h"
#include "MD_MaintenanceFrequencyCode.h"

#include <memory>


namespace S100 {
	class MD_MaintenanceInformation
	{
	public:
		std::shared_ptr<MD_MaintenanceFrequencyCode> MaintenanceAndUpdateFrequency;
		std::shared_ptr<CI_Date> MaintenanceDate;
		//UserDefinedMaintenanceFrequency

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "mmi:userDefinedMaintenanceFrequency"))
				{
					MaintenanceAndUpdateFrequency = std::make_shared<MD_MaintenanceFrequencyCode>(MD_MaintenanceFrequencyCodeFromString(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "mmi:maintenanceDate"))
				{
					CI_Date date;
					date.GetContents(instruction);
					MaintenanceDate = std::make_shared<CI_Date>(date);
				}
				else
				{
				}
			}
		}
	};

	

}

