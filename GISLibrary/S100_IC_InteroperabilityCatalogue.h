#pragma once
#include "CT_Catalogue.h"
#include "requirementType.h"
#include "S100_IC_DisplayPlane.h"
#include "S100_IC_PredefinedCombination.h"
#include "S100_IC_HybridFeatureCreationRule.h"
#include "dataProduct.h"
#include "XML_Collection.h"

namespace S100
{
	class S100_IC_InteroperabilityCatalogue : public CT_Catalogue
	{
	public:
		std::string Description;
		std::string* Comment = nullptr;
		int* InteroperabilityLevel = nullptr;
		requirementType RequirementType;
		std::string RequirementDescription;
		std::vector<dataProduct> ProductCovered;

		XmlCollection<S100_IC_DisplayPlane> DisplayPlanes;
		XmlCollection<S100_IC_PredefinedCombination> PredefinedProductCombinations;
		std::vector<S100_IC_HybridFeatureCreationRule> HybridizationRules;
		std::vector<std::string> HybridFC;
		std::vector<std::string> HybridPC;

		S100_IC_InteroperabilityCatalogue();
		~S100_IC_InteroperabilityCatalogue();

		void Open(std::string filePath);
		void GetContents(pugi::xml_node& node);

		//√ ±‚»≠
		void SetDescription(std::string value);
		std::string GetDescription();

		void SetComment(std::string value);
		std::string* GetComment();

		void SetInteroperabilityLevel(std::string value);
		int* GetInteroperabilityLevel();

		void SetRequirementType(requirementType Type);
		requirementType GetRequirementType();

		void SetRequirementDescription(std::string value);
		std::string GetRequirementDescription();

		void SetDisplayPlanes(pugi::xml_node& node);
		XmlCollection<S100_IC_DisplayPlane> GetDisplayPlanes();

		void SetPredefinedProductCombinations(pugi::xml_node& node);
		XmlCollection<S100_IC_PredefinedCombination> GetPredefinedProductCombinations();

		void SetHybridizationRules(pugi::xml_node& node);
		std::vector<S100_IC_HybridFeatureCreationRule> GetHybridizationRules();

		void SetHybridFC(pugi::xml_node& node);
		std::vector<std::string> GetHybridFC();

		void SetHybridPC(pugi::xml_node& node);
		std::vector<std::string> GetHybridPC();
	};
}

