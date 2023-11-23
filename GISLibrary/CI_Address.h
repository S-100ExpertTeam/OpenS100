#pragma once
#include "CT_Catalogue.h"

#include <string>
#include <vector>


namespace S100 
{
	class CI_Address
	{
	public:
		std::string Deliverypoint;
		std::string City;
		std::string Administrativearea;
		std::string Postalcode;
		std::string Country;
		std::vector<std::string> Electronicmailaddress;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:deliveryPoint"))
				{
					Deliverypoint = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cit:city"))
				{
					City = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cit:administrativeArea"))
				{
					Administrativearea = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cit:postalCode"))
				{
					Postalcode = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cit:country"))
				{
					Country = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cit:electronicMailAddress"))
				{
					Electronicmailaddress.push_back(CharacterString(instruction));
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