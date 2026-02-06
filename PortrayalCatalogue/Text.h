#pragma once
#include <string>
#include <vector>
#include <memory>

#include "pugixml.hpp"

namespace S100
{
	class Text
	{
	public:
		std::wstring Value;
		std::shared_ptr<std::string> Language;

		void GetContents(pugi::xml_node& node);
	};
}