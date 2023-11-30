#pragma once
#include <string>
#include <vector>
#include "pugixml.hpp"

namespace S100
{
	class ViewingGroupReference
	{
	public:
		std::string ref;

		void GetContents(pugi::xml_node& node);
	};
}
