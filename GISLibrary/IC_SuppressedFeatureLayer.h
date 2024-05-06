#pragma once
#include "IC_Feature.h"
#include "IC_DrawingInstruction.h"
#include "XML_Collection.h"

namespace S100 {
    class IC_SuppressedFeatureLayer : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string FeatureCode;
        dataProduct Product;
        std::vector<IC_Feature> FeatureRef;
        XmlCollection<IC_DrawingInstruction> DrawingInstructionRef;

        void GetContents(pugi::xml_node& node) override;
        void SaveXmlNode(pugi::xml_node& node) override;
    };
}
