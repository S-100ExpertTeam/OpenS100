#include "stdafx.h"
#include "S100_IC_InteroperabilityCatalogue.h"

namespace S100
{
    void S100_IC_InteroperabilityCatalogue::Open(std::string filePath)
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

    void S100_IC_InteroperabilityCatalogue::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

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
                VersionDate = S100_Date(CharacterString(instruction));
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
        }
    }
}
