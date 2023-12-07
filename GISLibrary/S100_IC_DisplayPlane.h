#pragma once

#include "S100_IC_DrawingInstruction.h"
#include "S100_IC_Feature.h"
#include "XML_Collection.h"

#include <memory>

namespace S100
{
    class S100_IC_DisplayPlane : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string Name;
        int Order;
        std::string Description;
        std::shared_ptr<int> InteroperabilityLevel;
        XmlCollection<S100_IC_Feature> Features;
        XmlCollection<S100_IC_DrawingInstruction> DrawingInstructions;

        void GetContents(pugi::xml_node& node) override;
    };
}
