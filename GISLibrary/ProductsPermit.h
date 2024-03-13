#pragma once
#include "ProductPermit.h"
namespace S100
{
	class ProductsPermit  {
	public:

		std::vector<ProductPermit> products;

		void GetContents(pugi::xml_node& node);
	};
}