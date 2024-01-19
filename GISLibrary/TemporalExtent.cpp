#include "stdafx.h"
#include "TemporalExtent.h"

namespace S100 {
    void TemporalExtent::GetContents(pugi::xml_node& node)
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

    void TemporalExtent::Save(pugi::xml_node& node)
    {
        if(TimeInstantBegin)
        {
            auto child = node.append_child("timeInstantBegin");
            child.text().set(TimeInstantBegin->ToString().c_str());
        }
        if (TimeInstantEnd)
        {
            auto child = node.append_child("timeInstantEnd");
            child.text().set(TimeInstantEnd->ToString().c_str());
        }
    }
}

