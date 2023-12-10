#include "stdafx.h"
#include "ProductSpecification.h"

namespace S100 {
    void ProductSpecification::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:name"))
            {
                Name = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:version"))
            {
                Version = std::make_shared<std::string>(instruction.child_value());
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
                compliancyCategory = std::make_shared<CompliancyCategory>(S100_CompliancyCategoryFromString(instruction.child_value()));
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}
