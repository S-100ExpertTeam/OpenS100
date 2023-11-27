#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Base64String.h"

namespace S100 {

	class S100_SE_CertificateContainerType
	{
	public :
		std::shared_ptr<std::string> SchemeAdministrator;
		std::vector<Base64String> Certificate;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "S100SE:schemeAdministrator"))
				{
					SchemeAdministrator = std::make_shared< std::string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100SE:certificate"))
				{
					Certificate.push_back(Base64String(instruction.child_value()));

				}
				else
				{
				}
			}
		}
	};

}