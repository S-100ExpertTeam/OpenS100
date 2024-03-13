#include "stdafx.h"
#include "EX_BoundingPolygon.h"
#include "pugiSupportClass.h"

namespace S100 {
    void EX_BoundingPolygon::GetContents(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "gex:extentTypeCode")) {
                ExtentTypeCode =std::make_shared<bool>(ExXmlSupport().ParseStr2Bool(instruction.child_value()));
            }
            else if (ExXmlSupport().Compare(instructionName, "polygon")) {
                GM_Object go;
                go.GetContents(instruction);
                Polygon = go;
            }
            else {
            }
        }
    }

    void EX_BoundingPolygon::Save(pugi::xml_node& node) {
        {
            if(ExtentTypeCode)
            {
                auto child = node.append_child("gex:extentTypeCode");
                child.text().set(ExXmlSupport().ParseBool2Str(*ExtentTypeCode).c_str());
            }
            {
                auto child = node.append_child("gex:polygon");
                Polygon.Save(child);
            }
        }
    }
}
