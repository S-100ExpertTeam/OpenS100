#include "stdafx.h"
#include "S100_IC_InteroperabilityCatalogue.h"
#include "pugixml.hpp"


namespace S100
{
	S100_IC_InteroperabilityCatalogue::S100_IC_InteroperabilityCatalogue()
	{

	}

	S100_IC_InteroperabilityCatalogue::~S100_IC_InteroperabilityCatalogue()
	{

	}


	void S100_IC_InteroperabilityCatalogue::Open(std::string filePath)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filePath.c_str());
		pugi::xml_node nodeList = doc.first_child();


		std::wstring nodeName = pugi::as_wide(nodeList.name());
		GetContents(nodeList);



		//return 
		//return true;
	}


	void S100_IC_InteroperabilityCatalogue::GetContents(pugi::xml_node& node)
	{
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();

			//CT_Catalogue

			if (!strcmp(instructionName, "cat:name"))
			{
				setName(CharacterString(instruction));
			}
			else if (!strcmp(instructionName, "cat:scope"))
			{
				setScope(CharacterString(instruction));
			}
			else if (!strcmp(instructionName, "cat:fieldOfApplication"))
			{
				setFieldOfApplication(CharacterString(instruction));
			}
			else if (!strcmp(instructionName, "cat:versionNumber"))
			{
				setVersionNumber(CharacterString(instruction));
			}
			else if (!strcmp(instructionName, "cat:versionDate"))
			{
				
				//setVersionDate(instruction);

			}
			else if (!strcmp(instructionName, "cat:language"))
			{
			}
			else if (!strcmp(instructionName, "cat:locale"))
			{
			}
			else if (!strcmp(instructionName, "cat:characterSet"))
			{
			}



			//S100_IC_InteroperabilityCatalogue


			else if (!strcmp(instructionName, "description"))
			{
			}
			else if (!strcmp(instructionName, "comment"))
			{
			}
			else if (!strcmp(instructionName, "interoperabilityLevel"))
			{
			}
			else if (!strcmp(instructionName, "requirementType"))
			{
			}
			else if (!strcmp(instructionName, "requirementDescription"))
			{
			}
			else if (!strcmp(instructionName, "displayPlanes"))
			{
			}
			else if (!strcmp(instructionName, "predefinedProductCombinations"))
			{
			}
			else if (!strcmp(instructionName, "hybridizationRules"))
			{
			}
			else if (!strcmp(instructionName, "hybridFC"))
			{
			}
			else if (!strcmp(instructionName, "hybridPC"))
			{
			}
			else
			{
				std::wstring content = pugi::as_wide(instructionName);
				content.append(L"is another data");
			}

		}
	}

}



