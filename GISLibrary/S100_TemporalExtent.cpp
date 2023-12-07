#include "stdafx.h"
#include "S100_TemporalExtent.h"

namespace S100 {
    void S100_TemporalExtent::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "timeInstantBegin"))
            {
                TimeInstantBegin = std::make_shared<S100_DateTime>(S100_DateTime(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "timeInstantEnd"))
            {
                TimeInstantEnd = std::make_shared<S100_DateTime>(S100_DateTime(instruction.child_value()));
            }
        }
    }
}
