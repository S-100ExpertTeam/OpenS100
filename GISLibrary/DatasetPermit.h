#pragma once
#include <string>
#include "S100_DateTime.h"
#include "S100_Date.h"
#include <pugixml.hpp>

namespace S100 {
	class DatasetPermit {
	public:
		std::string filename;
		int editionNumber;
		S100_DateTime issueDate;
		S100_Date expiry;
		std::string encryptedKey;

		void GetContents(pugi::xml_node& node);
	};
}