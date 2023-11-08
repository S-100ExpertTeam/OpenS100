#pragma once
#include "CT_Catalogue.h"
#include "requirementType.h"
#include "S100_IC_DisplayPlane.h"
#include "S100_IC_PredefinedCombination.h"
#include "S100_IC_HybridFeatureCreationRule.h"
#include "dataProduct.h"
#include "XML_Collection.h"
#include "S100_IC_HybridFC.h"
#include "S100_IC_HybridPC.h"
#include "pugixml.hpp"
#include "S100_IC_SimpleRule.h"
#include "S100_IC_ThematicRule.h"
#include "S100_IC_CompleteRule.h"


namespace S100
{
	class S100_IC_InteroperabilityCatalogue : public CT_Catalogue
	{
	public:
		std::string Description;
		std::shared_ptr<std::string> Comment = nullptr;
		std::shared_ptr<int> InteroperabilityLevel = nullptr;
		requirementType RequirementType;
		std::string RequirementDescription;
		std::vector<dataProduct> ProductCovered;

		XmlCollection<S100_IC_DisplayPlane> DisplayPlanes;
		XmlCollection<S100_IC_PredefinedCombination> PredefinedProductCombinations;
		std::vector<S100_IC_HybridFeatureCreationRule> HybridizationRules;
		std::vector<S100_IC_HybridFC> HybridFC;
		std::vector<S100_IC_HybridPC> HybridPC;

		inline void S100_IC_InteroperabilityCatalogue::Open(std::string filePath)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.c_str());
			pugi::xml_node nodeList = doc.first_child();

			const pugi::char_t* instructionName = nodeList.name();
			if (!strcmp(instructionName, "S100IC:S100_IC_InteroperabilityCatalogue"))
			{
				GetContents(nodeList);
			}
			else
			{

			}
		}

		inline void S100_IC_InteroperabilityCatalogue::GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				//CT_Catalogue

				if (!strcmp(instructionName, "cat:name"))
				{
					Name = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cat:scope"))
				{
					Scope = std::make_shared<std::string>(CharacterString(instruction));
				}
				else if (!strcmp(instructionName, "cat:fieldOfApplication"))
				{
					FieldOfApplication.push_back(CharacterString(instruction));
				}
				else if (!strcmp(instructionName, "cat:versionNumber"))
				{

					VersionNumber = CharacterString(instruction);
				}
				else if (!strcmp(instructionName, "cat:versionDate"))
				{
					CString cstrDate((CharacterString(instruction)).c_str());
					COleDateTime oleDateTime;
					if (oleDateTime.ParseDateTime(cstrDate, VAR_DATEVALUEONLY))
					{
						SYSTEMTIME st;
						if (oleDateTime.GetAsSystemTime(st))
						{
							tm tmDateTime;
							tmDateTime.tm_year = st.wYear - 1900;
							tmDateTime.tm_mon = st.wMonth - 1;
							tmDateTime.tm_mday = st.wDay;
							tmDateTime.tm_hour = st.wHour;
							tmDateTime.tm_min = st.wMinute;
							tmDateTime.tm_sec = st.wSecond;
							tmDateTime.tm_isdst = -1;

							VersionDate = mktime(&tmDateTime);
						}
					}
				}
				else if (!strcmp(instructionName, "cat:language"))
				{
					Language = std::make_shared<std::string>(CharacterString(instruction));
				}
				else if (!strcmp(instructionName, "cat:locale"))
				{
					auto loc = std::make_shared<PT_Locale>();
					loc->GetContents(instruction);
					Locale.push_back(loc);
				}
				else if (!strcmp(instructionName, "cat:characterEncoding"))
				{
					CharacterSet = parseMD_CharacterSetCode(instruction);
				}


				//S100_IC_InteroperabilityCatalogue

				else if (!strcmp(instructionName, "description"))
				{
					Description = instruction.child_value();
				}
				else if (!strcmp(instructionName, "comment"))
				{
					Comment = std::make_shared<std::string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "interoperabilityLevel"))
				{
					InteroperabilityLevel = std::make_shared<int>(atoi(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "requirementType"))
				{
					RequirementType = stringTorequirementType(instruction.child_value());
				}
				else if (!strcmp(instructionName, "requirementDescription"))
				{
					RequirementDescription = instruction.child_value();
					//SetRequirementDescription(instruction.child_value());
				}
				else if (!strcmp(instructionName, "productCovered"))
				{
					ProductCovered.push_back(instruction.child_value());
				}
				else if (!strcmp(instructionName, "displayPlanes"))
				{
					DisplayPlanes.parseXmlNode(instruction, "S100_IC_DisplayPlane");
				}
				else if (!strcmp(instructionName, "predefinedProductCombinations"))
				{
					PredefinedProductCombinations.parseXmlNode(instruction, "S100_IC_PredefinedCombination");
				}
				else if (!strcmp(instructionName, "hybridizationRules"))
				{
					for (pugi::xml_node inst = instruction.first_child(); inst; inst = inst.next_sibling())
					{
						const pugi::char_t* instructionName = inst.name();

						if (!strcmp(instructionName, "S100_IC_SimpleRule"))
						{
							S100_IC_SimpleRule simple;
							simple.GetContents(inst);
							HybridizationRules.push_back((S100_IC_HybridFeatureCreationRule)simple);
						}
						else if (!strcmp(instructionName, "S100_IC_ThematicRule"))
						{
							S100_IC_ThematicRule them;
							them.GetContents(inst);
							HybridizationRules.push_back((S100_IC_HybridFeatureCreationRule)them);
						}
						else if (!strcmp(instructionName, "S100_IC_CompleteRule"))
						{
							S100_IC_CompleteRule comp;
							comp.GetContents(inst);
							HybridizationRules.push_back((S100_IC_HybridFeatureCreationRule)comp);
						}
						else
						{
							std::wstring content = pugi::as_wide(instructionName);
							content.append(L"is another data");
						}
					}
				}
				else if (!strcmp(instructionName, "hybridFC"))
				{
					S100_IC_HybridFC fc;
					fc.GetContents(node);
					HybridFC.push_back(fc);
				}
				else if (!strcmp(instructionName, "hybridPC"))
				{
					S100_IC_HybridPC pc;
					pc.GetContents(node);
					HybridPC.push_back(pc);
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

