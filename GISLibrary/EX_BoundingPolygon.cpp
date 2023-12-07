#include "stdafx.h"
#include "EX_BoundingPolygon.h"
#include "pugiSupportClass.h"

namespace S100 {
    void EX_BoundingPolygon::GetContents(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "gex:extentTypeCode")) {
                ExtentTypeCode = ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "gex:polygon")) {
                GM_Object go;
                go.GetContents(instruction);
                Polygon = go;
            }
            else {
            }
        }
    }
}
