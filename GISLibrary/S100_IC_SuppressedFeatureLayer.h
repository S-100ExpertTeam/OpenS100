#pragma once
#include "S100_IC_Feature.h"
#include "S100_IC_DrawingInstruction.h"
#include "XML_Collection.h"

namespace S100 {
    class S100_IC_SuppressedFeatureLayer : public xmlParseable
    {
    public:
        std::string Identifier;
        std::string FeatureCode;
        dataProduct Product;
        std::vector<S100_IC_Feature> FeatureRef;
        XmlCollection<S100_IC_DrawingInstruction> DrawingInstructionRef;

        void GetContents(pugi::xml_node& node) override;
    };
}
