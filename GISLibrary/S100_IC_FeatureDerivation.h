#pragma once

#include "S100_IC_Feature.h"
#include "FeatureSelector.h"
#include "XML_Collection.h"

#include <memory>

namespace S100
{
	class S100_IC_FeatureDerivation : public xmlParseable
	{
	public:
		std::string Identifier;
		dataProduct PrimaryProduct;
		std::string PrimaryFeatureCode;
		std::shared_ptr<FeatureSelector> PrimarySelector;
		dataProduct SecondaryProduct;
		std::string SecondaryFeatureCode;
		std::shared_ptr<FeatureSelector> SecondarySelector;
		dataProduct OutputProduct;
		std::string OutputFeatureCode;
		std::shared_ptr<S100_IC_Feature> FeatureRef;

		inline void GetContents(pugi::xml_node& node) override
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "identifier"))
				{
					Identifier = instruction.child_value();
				}
				else if (!strcmp(instructionName, "primaryProduct"))
				{
					PrimaryProduct = instruction.child_value();
				}
				else if (!strcmp(instructionName, "primaryFeatureCode"))
				{
					PrimaryFeatureCode = atoi(instruction.child_value());
				}
				else if (!strcmp(instructionName, "primarySelector"))
				{
					PrimarySelector = std::make_shared<FeatureSelector>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "secondaryProduct"))
				{
					SecondaryProduct = instruction.child_value();
				}
				else if (!strcmp(instructionName, "secondaryFeatureCode"))
				{
					SecondaryFeatureCode = instruction.child_value();
				}
				else if (!strcmp(instructionName, "secondarySelector"))
				{
					SecondarySelector = std::make_shared<FeatureSelector>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "outputProduct"))
				{
					OutputProduct = instruction.child_value();
				}
				else if (!strcmp(instructionName, "outputFeatureCode"))
				{
					OutputFeatureCode = instruction.child_value();
				}
				else if (!strcmp(instructionName, "featureRef"))
				{
					FeatureRef = std::make_shared<S100_IC_Feature>();
					FeatureRef->GetContents (instruction);
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


