#include "stdafx.h"
#include "EX_GeographicBoundingBox.h"

namespace S100 {
    void EX_GeographicBoundingBox::GetContents(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (ExXmlSupport().Compare(instructionName, "westBoundLongitude")) {
                WestBoundLongitude = atof(std::string(ExXmlSupport().GetContentItem(instruction, "gco:Decimal")).c_str());
            }
            else if (ExXmlSupport().Compare(instructionName, "eastBoundLongitude")) {
                EastBoundLongitude = atof(std::string(ExXmlSupport().GetContentItem(instruction, "gco:Decimal")).c_str());
            }
            else if (ExXmlSupport().Compare(instructionName, "southBoundLatitude")) {
                SouthBoundLatitude = atof(std::string(ExXmlSupport().GetContentItem(instruction, "gco:Decimal")).c_str());
            }
            else if (ExXmlSupport().Compare(instructionName, "northBoundLatitude")) {
                NorthBoundLatitude = atof(std::string(ExXmlSupport().GetContentItem(instruction, "gco:Decimal")).c_str());
            }
        }
    }

    void EX_GeographicBoundingBox::Save(pugi::xml_node& node)
    {
        {
            auto child = node.append_child("gex:westBoundLongitude");
            auto item = child.append_child("gco:Decimal");
            item.text().set(std::to_string(WestBoundLongitude).c_str());
        }
        {
            auto child = node.append_child("gex:eastBoundLongitude");
            auto item = child.append_child("gco:Decimal");
            item.text().set(std::to_string(EastBoundLongitude).c_str());
        }
        {
            auto child = node.append_child("gex:southBoundLatitude");
            auto item = child.append_child("gco:Decimal");
            item.text().set(std::to_string(SouthBoundLatitude).c_str());
        }
        {
            auto child = node.append_child("gex:northBoundLatitude");
            auto item = child.append_child("gco:Decimal");
            item.text().set(std::to_string(NorthBoundLatitude).c_str());
        }
    }
}
