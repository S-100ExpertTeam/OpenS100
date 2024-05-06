#pragma once

#include <map>
#include <string>
#include "pugixml.hpp"

namespace S100 {
    enum class MD_CharacterSetCode {
        ucs2,
        ucs4,
        utf7,
        utf8,
        utf16,
        _8859part1,
        _8859part2,
        _8859part3,
        _8859part4,
        _8859part5,
        _8859part6,
        _8859part7,
        _8859part8,
        _8859part9,
        _8859part10,
        _8859part11,
        _8859part12,
        _8859part13,
        _8859part14,
        _8859part15,
        _8859part16,
        jis,
        shiftJIS,
        eucJP,
        usAscii,
        ebcdic,
        eucKR,
        big5,
        GB2312,
    };

    std::string MD_CharacterSetCodeToString(MD_CharacterSetCode category);
    MD_CharacterSetCode MD_CharacterSetCodeFromString(const std::string& categoryName);

    /*const std::map<std::string, MD_CharacterSetCode> stringToCodeMap = {
     {"ucs2", MD_CharacterSetCode::ucs2},
     {"ucs4", MD_CharacterSetCode::ucs4},
     {"utf7", MD_CharacterSetCode::utf7},
     {"utf8", MD_CharacterSetCode::utf8},
     {"utf16", MD_CharacterSetCode::utf16},
     {"_8859part1", MD_CharacterSetCode::_8859part1},
     {"_8859part2", MD_CharacterSetCode::_8859part2},
     {"_8859part3", MD_CharacterSetCode::_8859part3},
     {"_8859part4", MD_CharacterSetCode::_8859part4},
     {"_8859part5", MD_CharacterSetCode::_8859part5},
     {"_8859part6", MD_CharacterSetCode::_8859part6},
     {"_8859part7", MD_CharacterSetCode::_8859part7},
     {"_8859part8", MD_CharacterSetCode::_8859part8},
     {"_8859part9", MD_CharacterSetCode::_8859part9},
     {"_8859part10", MD_CharacterSetCode::_8859part10},
     {"_8859part11", MD_CharacterSetCode::_8859part11},
     {"_8859part13", MD_CharacterSetCode::_8859part13},
     {"_8859part14", MD_CharacterSetCode::_8859part14},
     {"_8859part15", MD_CharacterSetCode::_8859part15},
     {"_8859part16", MD_CharacterSetCode::_8859part16},
     {"jis", MD_CharacterSetCode::jis},
     {"shiftJIS", MD_CharacterSetCode::shiftJIS},
     {"eucJP", MD_CharacterSetCode::eucJP},
     {"usAscii", MD_CharacterSetCode::usAscii},
     {"ebcdic", MD_CharacterSetCode::ebcdic},
     {"eucKR", MD_CharacterSetCode::eucKR},
     {"big5", MD_CharacterSetCode::big5},
     {"GB2312", MD_CharacterSetCode::GB2312}
    };


    MD_CharacterSetCode stringToMD_CharacterSetCode(const std::string& codeName);
    MD_CharacterSetCode parseMD_CharacterSetCode(pugi::xml_node& node);*/
}
