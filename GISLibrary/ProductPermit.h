#pragma once
#include <string>
#include "DatasetPermit.h"
namespace S100
{
	class ProductPermit {
	public:
		//attribute
		std::string id;
		std::vector<DatasetPermit> datasetPermit;

		void GetContents(pugi::xml_node& node);
	};
}