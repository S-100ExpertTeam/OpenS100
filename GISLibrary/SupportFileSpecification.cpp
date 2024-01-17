#include "stdafx.h"
#include "SupportFileSpecification.h"

namespace S100 {
    void SupportFileSpecification::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:name"))
            {
                Name = instruction.child_value();
            }
            else if (!strcmp(instructionName, "S100XC:contact"))
            {
                Version = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:date"))
            {
                Date = std::make_shared<S100_Date>(S100_Date(instruction.child_value()));
            }
        }
    }
}
