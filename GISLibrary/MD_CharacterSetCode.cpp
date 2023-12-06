#include "stdafx.h"
#include "MD_CharacterSetCode.h"

namespace S100 {
 
    MD_CharacterSetCode stringToMD_CharacterSetCode(const std::string& codeName) {
        auto it = stringToCodeMap.find(codeName);
        if (it != stringToCodeMap.end()) {
            return it->second;
        }
        return MD_CharacterSetCode::eucKR;
    }

    MD_CharacterSetCode parseMD_CharacterSetCode(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "lan:MD_CharacterSetCode")) {
                return stringToMD_CharacterSetCode(instruction.child_value());
            }
        }
        return MD_CharacterSetCode::eucKR;
    }
}
