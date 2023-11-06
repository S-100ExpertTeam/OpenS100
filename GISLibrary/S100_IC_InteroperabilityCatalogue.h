#pragma once
#include "CT_Catalogue.h"
#include "requirementType.h"
#include "S100_IC_DisplayPlane.h"
#include "S100_IC_PredefinedCombination.h"
#include "S100_IC_HybridFeatureCreationRule.h"
namespace S100
{
	//@1
	class S100_IC_InteroperabilityCatalogue : public CT_Catalogue
	{
	public:
		std::string Description;
		std::string* Comment = nullptr;
		int* InteroperabilityLevel = nullptr;
		requirementType RequirementType;
		std::string RequirementDescription;
		//ProductCovered;

		std::vector<S100_IC_DisplayPlane*> DisplayPlanes;
		S100_IC_PredefinedCombination* PredefinedProductCombinations;
		S100_IC_HybridFeatureCreationRule* HybridizationRules;
		std::vector<std::string> HybridFC;
		std::vector<std::string> HybridPC;

		S100_IC_InteroperabilityCatalogue();
		~S100_IC_InteroperabilityCatalogue();

		void Open(std::string filePath);
		void GetContents(pugi::xml_node& node);

		//√ ±‚»≠
		void SetName(char* value);
		std::string GetName();

	};
}

