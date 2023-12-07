#include "stdafx.h"
#include "dataProduct.h"

namespace S100 {
    void dataProduct::GetContents(pugi::xml_node& node, std::string nodeName) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, nodeName.c_str())) {
                *this = std::string(instruction.child_value());
            }
        }
    }
}
