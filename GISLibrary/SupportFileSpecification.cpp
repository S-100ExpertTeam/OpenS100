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

    void SupportFileSpecification::Save(pugi::xml_node& node)
    {
        if (!Name.empty())
        {
            auto child = node.append_child("S100XC:name");
            child.text().set(Name.c_str());
        }
        if (Version)
        {
            auto child = node.append_child("S100XC:contact");
            child.text().set(Version->c_str());
        }
        if (Date)
        {
            auto child = node.append_child("S100XC:date");
            child.text().set(Date->ToString().c_str());
        }
    }
}
