#include "stdafx.h"
#include "S100_IC_InteroperabilityCatalogue.h"
#include "pugixml.hpp"
#include "S100_IC_SimpleRule.h"
#include "S100_IC_ThematicRule.h"
#include "S100_IC_CompleteRule.h"


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
				setVersionDate(CharacterString(instruction));
			}
			else if (!strcmp(instructionName, "cat:language"))
			{
				setLanguage(CharacterString(instruction));
			}
			else if (!strcmp(instructionName, "cat:locale"))
			{
				setLocale(instruction);
			}
			else if (!strcmp(instructionName, "cat:characterEncoding"))
			{
				parseMD_CharacterSetCode(instruction);
			}


			//S100_IC_InteroperabilityCatalogue

			else if (!strcmp(instructionName, "description"))
			{
				SetDescription(instruction.child_value());
			}
			else if (!strcmp(instructionName, "comment"))
			{
				SetComment(instruction.child_value());
			}
			else if (!strcmp(instructionName, "interoperabilityLevel"))
			{
				SetInteroperabilityLevel(instruction.child_value());
			}
			else if (!strcmp(instructionName, "requirementType"))
			{
				SetRequirementType(stringTorequirementType(instruction.child_value()));
			}
			else if (!strcmp(instructionName, "requirementDescription"))
			{
				SetRequirementDescription(instruction.child_value());
			}
			else if (!strcmp(instructionName, "productCovered"))
			{
				ProductCovered.push_back(instruction.child_value());
			}
			else if (!strcmp(instructionName, "displayPlanes"))
			{
				SetDisplayPlanes(instruction);
			}
			else if (!strcmp(instructionName, "predefinedProductCombinations"))
			{
				SetPredefinedProductCombinations(instruction);
			}
			else if (!strcmp(instructionName, "hybridizationRules"))
			{
				SetHybridizationRules(instruction);
			}
			else if (!strcmp(instructionName, "hybridFC"))
			{
				SetHybridFC(instruction);
			}
			else if (!strcmp(instructionName, "hybridPC"))
			{
				SetHybridPC(instruction);
			}
			else
			{
				std::wstring content = pugi::as_wide(instructionName);
				content.append(L"is another data");
			}
		}
	}

	void S100_IC_InteroperabilityCatalogue::SetDescription(std::string value) {
		Description = value;
	}

	std::string S100_IC_InteroperabilityCatalogue::GetDescription()	{
		return Description;
	}

	void S100_IC_InteroperabilityCatalogue::SetComment(std::string value) {
		Comment = new std::string(value);
	}
	std::string* S100_IC_InteroperabilityCatalogue::GetComment(){
		return Comment;
	}

	void S100_IC_InteroperabilityCatalogue::SetInteroperabilityLevel(std::string value) {
		InteroperabilityLevel = new int(atoi(value.c_str()));
	}

	int* S100_IC_InteroperabilityCatalogue::GetInteroperabilityLevel() {
		return InteroperabilityLevel;
	}

	void S100_IC_InteroperabilityCatalogue::SetRequirementType(requirementType Type) {
		RequirementType = Type;
	}
	requirementType S100_IC_InteroperabilityCatalogue::GetRequirementType() {
		return RequirementType;
	}

	void S100_IC_InteroperabilityCatalogue::SetRequirementDescription(std::string value) {
		RequirementDescription = value;
	}
	std::string S100_IC_InteroperabilityCatalogue::GetRequirementDescription() {
		return RequirementDescription;
	}

	void S100_IC_InteroperabilityCatalogue::SetDisplayPlanes(pugi::xml_node& node) {
		DisplayPlanes.parseXmlNode(node, "S100_IC_DisplayPlane");
	}
	XmlCollection<S100_IC_DisplayPlane> S100_IC_InteroperabilityCatalogue::GetDisplayPlanes() {
		return DisplayPlanes;
	}

	void S100_IC_InteroperabilityCatalogue::SetPredefinedProductCombinations(pugi::xml_node& node) {
		PredefinedProductCombinations.parseXmlNode(node, "S100_IC_PredefinedCombination");
	}
	XmlCollection<S100_IC_PredefinedCombination> S100_IC_InteroperabilityCatalogue::GetPredefinedProductCombinations() {
		return PredefinedProductCombinations;
	}

	void S100_IC_InteroperabilityCatalogue::SetHybridizationRules(pugi::xml_node& node) {
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();

			if (!strcmp(instructionName, "S100_IC_SimpleRule"))
			{
				S100_IC_SimpleRule simple;
				simple.GetContents(instruction);
				HybridizationRules.push_back((S100_IC_HybridFeatureCreationRule)simple);
			}
			else if (!strcmp(instructionName, "S100_IC_ThematicRule"))
			{
				S100_IC_ThematicRule them;
				them.GetContents(instruction);
				HybridizationRules.push_back((S100_IC_HybridFeatureCreationRule)them);
			}
			else if (!strcmp(instructionName, "S100_IC_CompleteRule"))
			{
				S100_IC_CompleteRule comp;
				comp.GetContents(instruction);
				HybridizationRules.push_back((S100_IC_HybridFeatureCreationRule)comp);
			}
			else
			{
				std::wstring content = pugi::as_wide(instructionName);
				content.append(L"is another data");
			}
		}

	}
	std::vector<S100_IC_HybridFeatureCreationRule> S100_IC_InteroperabilityCatalogue::GetHybridizationRules() {
		return HybridizationRules;
	}


	void S100_IC_InteroperabilityCatalogue::SetHybridFC(pugi::xml_node& node) {

	}
	std::vector<std::string> S100_IC_InteroperabilityCatalogue::GetHybridFC() {
		return HybridFC;
	}

	void S100_IC_InteroperabilityCatalogue::SetHybridPC(pugi::xml_node& node) {

	}
	std::vector<std::string> S100_IC_InteroperabilityCatalogue::GetHybridPC() {
		return HybridPC;
	}



}



