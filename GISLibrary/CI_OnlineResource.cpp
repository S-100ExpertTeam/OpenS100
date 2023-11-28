#include "stdafx.h"
#include "CI_OnlineResource.h"

namespace S100
{
    void CI_OnlineResource::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:linkage"))
            {
                Linkage = instruction.child_value();
            }
            else if (!strcmp(instructionName, "cit:protocol"))
            {
                Protocol = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:applicationProfile"))
            {
                ApplicationProfile = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:name"))
            {
                Name = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:description"))
            {
                Description = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:function"))
            {
                Function = std::make_shared<CI_OnLineFunctionCode>(CI_OnLineFunctionCodeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "cit:protocolRequest"))
            {
                ProtocolRequest = std::make_shared<std::string>(instruction.child_value());
            }
        }
    }
}
