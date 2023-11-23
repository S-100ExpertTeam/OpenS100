#pragma once
#include <memory>
#include <string>
#include <time.h>
#include "S100_CompliancyCategory.h"
#include "S100_Date.h"

namespace S100 {
	class S100_ProductSpecification
	{
	public:
		std::shared_ptr<std::string> Name;
		std::shared_ptr<std::string> Version;
		std::shared_ptr<S100_Date> Date;
		std::string ProductIdentifier;
		int Number;
		std::shared_ptr<S100_CompliancyCategory> CompliancyCategory;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "S100XC:name"))
				{
					Name = std::make_shared < std::string >(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:version"))
				{
					Version = std::make_shared < std::string >(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:date"))
				{
					Date = std::make_shared<S100_Date>(S100_Date(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "S100XC:productIdentifier"))
				{
					ProductIdentifier = instruction.child_value();
				}
				else if (!strcmp(instructionName, "S100XC:number"))
				{
					Number = atoi(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:compliancyCategory"))
				{
					CompliancyCategory = std::make_shared< S100_CompliancyCategory>(S100_CompliancyCategoryFromString(instruction.child_value()));
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