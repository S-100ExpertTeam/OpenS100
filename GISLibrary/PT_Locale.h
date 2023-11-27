#pragma once
#include "LanguageCode.h"
#include "CountryCode.h"
#include "pugixml.hpp"
#include "MD_CharacterSetCode.h"


namespace S100
{
	class PT_Locale
	{
	public:
		LanguageCode Language;
		CountryCode* Country;
		MD_CharacterSetCode CharacterEncoding;

		inline void setLanguage(pugi::xml_node& node)
		{
			Language.GetContents(node);
		}

		inline void setCountry(pugi::xml_node& node)
		{
			Country = new CountryCode();
			Country->GetContents(node);
		}

		inline void setCharacterEncoding(pugi::xml_node& node)
		{
 			CharacterEncoding = parseMD_CharacterSetCode(node);
		}

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "lan:language"))
				{
					setLanguage(instruction);
				}
				else if (!strcmp(instructionName, "lan:country"))
				{
					setCountry(instruction);
				}
				else if (!strcmp(instructionName, "lan:characterEncoding"))
				{
					setCharacterEncoding(instruction);
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