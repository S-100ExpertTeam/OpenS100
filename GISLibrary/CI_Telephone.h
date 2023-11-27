#pragma once

#include "CT_Catalogue.h"

#include <string>
#include <vector>

namespace S100 {

	class CI_Telephone
	{
	public:
		std::string Number;
		std::string Numbertype;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:number"))
				{
					Number = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cit::numbertype"))
				{
					Numbertype = CharacterString(instruction);
				}
				else
				{
					std::wstring content = pugi::as_wide(instructionName);
					content.append(L"is another data");
				}
			}
		}

	};
}
