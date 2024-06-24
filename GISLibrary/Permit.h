#pragma once
#include "PermitHeader.h"
#include "ProductsPermit.h"

namespace S100 {

	class Permit
	{
	public:
		PermitHeader permitHeader;
		ProductsPermit productsPermit;

		void GetContents(pugi::xml_node& node);
		void Open(std::string filePath);
	};
}