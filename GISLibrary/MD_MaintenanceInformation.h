#pragma once

#include <memory>
#include "MD_MaintenanceFrequencyCode.h"
#include "CI_Date.h"
#include "pugixml.hpp"

namespace S100 {
	class MD_MaintenanceInformation
	{
	public:
		std::shared_ptr<MD_MaintenanceFrequencyCode> MaintenanceAndUpdateFrequency;
		std::shared_ptr<CI_Date> MaintenanceDate;
		//UserDefinedMaintenanceFrequency

		void GetContents(pugi::xml_node& node);
		void Save(pugi::xml_node& node);
	};
}


