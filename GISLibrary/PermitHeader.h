#pragma once
#include "S100_DateTime.h"
#include <pugixml.hpp>
#include "pugiSupportClass.h"

namespace S100 {
	class PermitHeader {
	public:
		S100_DateTime issueDate;
		std::string dataServerName;
		std::string dataServerIdentifier;
		std::string version;
		std::string userpermit;

		void GetContents(pugi::xml_node& node);
	};
}