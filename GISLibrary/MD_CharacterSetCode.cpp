#include "stdafx.h"
#include "MD_CharacterSetCode.h"
#include <stdexcept>

namespace S100 {
 
    //MD_CharacterSetCode stringToMD_CharacterSetCode(const std::string& codeName) {
    //    auto it = stringToCodeMap.find(codeName);
    //    if (it != stringToCodeMap.end()) {
    //        return it->second;
    //    }
    //    return MD_CharacterSetCode::eucKR;
    //}

    //MD_CharacterSetCode parseMD_CharacterSetCode(pugi::xml_node& node) {
    //    for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
    //        const pugi::char_t* instructionName = instruction.name();

    //        if (!strcmp(instructionName, "lan:MD_CharacterSetCode")) {
    //            return stringToMD_CharacterSetCode(instruction.child_value());
    //        }
    //    }
    //    return MD_CharacterSetCode::eucKR;
    //}

    std::string MD_CharacterSetCodeToString(MD_CharacterSetCode category)
    {
        switch (category) {
        case MD_CharacterSetCode::ucs2: return "ucs2";
        case MD_CharacterSetCode::ucs4: return "ucs4";
        case MD_CharacterSetCode::utf7: return "utf7";
        case MD_CharacterSetCode::utf8: return "utf8";
        case MD_CharacterSetCode::utf16: return "utf16";
        case MD_CharacterSetCode::_8859part1: return "_8859part1";
        case MD_CharacterSetCode::_8859part2: return "_8859part2";
        case MD_CharacterSetCode::_8859part3: return "_8859part3";
        case MD_CharacterSetCode::_8859part4: return "_8859part4";
        case MD_CharacterSetCode::_8859part5: return "_8859part5";
        case MD_CharacterSetCode::_8859part6: return "_8859part6";
        case MD_CharacterSetCode::_8859part7: return "_8859part7";
        case MD_CharacterSetCode::_8859part8: return "_8859part8";
        case MD_CharacterSetCode::_8859part9: return "_8859part9";
        case MD_CharacterSetCode::_8859part10: return "_8859part10";
        case MD_CharacterSetCode::_8859part11: return "_8859part11";
        case MD_CharacterSetCode::_8859part12: return "_8859part12";
        case MD_CharacterSetCode::_8859part13: return "_8859part13";
        case MD_CharacterSetCode::_8859part14: return "_8859part14";
        case MD_CharacterSetCode::_8859part15: return "_8859part15";
        case MD_CharacterSetCode::_8859part16: return "_8859part16";
        case MD_CharacterSetCode::jis: return "jis";
        case MD_CharacterSetCode::shiftJIS: return "shiftJIS";
        case MD_CharacterSetCode::eucJP: return "eucJP";
        case MD_CharacterSetCode::usAscii: return "usAscii";
        case MD_CharacterSetCode::ebcdic: return "ebcdic";
        case MD_CharacterSetCode::eucKR: return "eucKR";
        case MD_CharacterSetCode::big5: return "big5";
        case MD_CharacterSetCode::GB2312: return "GB2312";
        default: throw std::invalid_argument("Unknown category");
        }
    }
    MD_CharacterSetCode MD_CharacterSetCodeFromString(const std::string& categoryName)
    {
        if (categoryName == "ucs2") return MD_CharacterSetCode::ucs2;
        if (categoryName == "ucs4") return MD_CharacterSetCode::ucs4;
        if (categoryName == "utf7") return MD_CharacterSetCode::utf7;
        if (categoryName == "utf8") return MD_CharacterSetCode::utf8;
        if (categoryName == "utf16") return MD_CharacterSetCode::utf16;
        if (categoryName == "_8859part1") return MD_CharacterSetCode::_8859part1;
        if (categoryName == "_8859part2") return MD_CharacterSetCode::_8859part2;
        if (categoryName == "_8859part3") return MD_CharacterSetCode::_8859part3;
        if (categoryName == "_8859part4") return MD_CharacterSetCode::_8859part4;
        if (categoryName == "_8859part5") return MD_CharacterSetCode::_8859part5;
        if (categoryName == "_8859part6") return MD_CharacterSetCode::_8859part6;
        if (categoryName == "_8859part7") return MD_CharacterSetCode::_8859part7;
        if (categoryName == "_8859part8") return MD_CharacterSetCode::_8859part8;
        if (categoryName == "_8859part9") return MD_CharacterSetCode::_8859part9;
        if (categoryName == "_8859part10") return MD_CharacterSetCode::_8859part10;
        if (categoryName == "_8859part11") return MD_CharacterSetCode::_8859part11;
        if (categoryName == "_8859part12") return MD_CharacterSetCode::_8859part12;
        if (categoryName == "_8859part13") return MD_CharacterSetCode::_8859part13;
        if (categoryName == "_8859part14") return MD_CharacterSetCode::_8859part14;
        if (categoryName == "_8859part15") return MD_CharacterSetCode::_8859part15;
        if (categoryName == "_8859part16") return MD_CharacterSetCode::_8859part16;
        if (categoryName == "jis") return MD_CharacterSetCode::jis;
        if (categoryName == "shiftJIS") return MD_CharacterSetCode::shiftJIS;
        if (categoryName == "eucJP") return MD_CharacterSetCode::eucJP;
        if (categoryName == "usAscii") return MD_CharacterSetCode::usAscii;
        if (categoryName == "ebcdic") return MD_CharacterSetCode::ebcdic;
        if (categoryName == "eucKR") return MD_CharacterSetCode::eucKR;
        if (categoryName == "big5") return MD_CharacterSetCode::big5;
        if (categoryName == "GB2312") return MD_CharacterSetCode::GB2312;

        throw std::invalid_argument("Unknown category name");
    }
}
