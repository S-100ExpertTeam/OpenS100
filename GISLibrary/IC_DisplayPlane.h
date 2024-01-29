#pragma once

#include "IC_DrawingInstruction.h"
#include "IC_Feature.h"
#include "XML_Collection.h"

#include <memory>

namespace S100
{
    class IC_DisplayPlane : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string Name;
        int Order;
        std::string Description;
        std::shared_ptr<int> InteroperabilityLevel;
        XmlCollection<IC_Feature> Features;
        XmlCollection<IC_DrawingInstruction> DrawingInstructions;

        void GetContents(pugi::xml_node& node) override;
        void SaveXmlNode(pugi::xml_node& node) override;
    };
}
