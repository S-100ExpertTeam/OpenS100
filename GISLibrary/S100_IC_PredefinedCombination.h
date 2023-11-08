#pragma once
#include <string>
#include "S100_IC_DisplayPlane.h"
#include "S100_IC_SuppressedFeatureLayer.h"
#include "dataProduct.h"
#include "S100_IC_FeatureDerivation.h"
#include "XML_Collection.h"

namespace S100 {
	class S100_IC_PredefinedCombination : public xmlParseable
	{
	public:
		std::string Identifier;
		std::string Name;
		std::string Description;
		std::string UseConditions;
		int* InteroperabilityLevel;
		std::vector<dataProduct> IncludedProduct;
		XmlCollection<S100_IC_DisplayPlane> DisplayPlaneRef;
		XmlCollection<S100_IC_FeatureDerivation> DerivedFeatures;
		XmlCollection<S100_IC_SuppressedFeatureLayer> SuppressedFeatureLayers;


		inline void GetContents(pugi::xml_node& node) override
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "identifier"))
				{
					Identifier = instruction.child_value();
				}
				else if (!strcmp(instructionName, "name"))
				{
					Name = instruction.child_value();
				}
				else if (!strcmp(instructionName, "description"))
				{
					Description = instruction.child_value();
				}
				else if (!strcmp(instructionName, "useConditions"))
				{
					UseConditions = instruction.child_value();
				}
				else if (!strcmp(instructionName, "interoperabilityLevel"))
				{
					InteroperabilityLevel = new int(atoi(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "includedProduct"))
				{
					IncludedProduct.push_back(instruction.child_value());
				}
				else if (!strcmp(instructionName, "displayPlaneRef"))
				{
					DisplayPlaneRef.parseXmlNode(instruction, "S100_IC_DisplayPlane");
				}
				else if (!strcmp(instructionName, "derivedFeatures"))
				{
					DerivedFeatures.parseXmlNode(instruction, "S100_IC_FeatureDerivation");
				}
				else if (!strcmp(instructionName, "suppressedFeatureLayers"))
				{
					SuppressedFeatureLayers.parseXmlNode(instruction, "S100_IC_SuppressedFeatureLayer");
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

