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

    void ProductSpecification::Save(pugi::xml_node& node)
    {
        if (Name || Version || Date || !ProductIdentifier.empty() || compliancyCategory)
        {
            if (Name)
            {
                auto child = node.append_child("S100XC:name");
                child.text().set(Name->c_str());
            }
            if (Version)
            {
                auto child = node.append_child("S100XC:version");
                child.text().set(Version->c_str());
            }
            if (Date)
            {
                auto child = node.append_child("S100XC:date");
                child.text().set(Date->ToString().c_str());
            }
            if (!ProductIdentifier.empty())
            {
                auto child = node.append_child("S100XC:productIdentifier");
                child.text().set(ProductIdentifier.c_str());
            }
            {
                auto child = node.append_child("S100XC:number");
                child.text().set(std::to_string(Number).c_str());
            }
            if (compliancyCategory)
            {
                auto child = node.append_child("S100XC:compliancyCategory");
                child.text().set(S100_CompliancyCategoryToString(*compliancyCategory).c_str());
            }
        }
    }
}
