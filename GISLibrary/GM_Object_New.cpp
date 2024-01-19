#include "stdafx.h"
#include "GM_Object_New.h"
#include "pugiSupportClass.h"

namespace S100 {
    void GM_Object::GetContents(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            std::vector<std::string> words;
            std::stringstream sstream((std::string)instructionName);
            std::string word;
            while (getline(sstream, word, ':')) {
                words.push_back(word);
            }

            if (!strcmp(words[1].c_str(), "point") || !strcmp(words[1].c_str(), "multiPoint") ||
                !strcmp(words[1].c_str(), "LineString") || !strcmp(words[1].c_str(), "curve") ||
                !strcmp(words[1].c_str(), "multiCurve")) {
                Geom.push_back(GetGeoms(instruction));
            }
            else if (!strcmp(words[1].c_str(), "polygon")) {
                auto temp = GetContentNode(instruction, "LineString", "gml");
                Geom.push_back(GetGeoms(temp));
            }
        }
    }

    std::string GM_Object::GetGeoms(pugi::xml_node& node) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            int tempIdx = 1;
            std::vector<std::string> words;
            std::stringstream sstream((std::string)instructionName);
            std::string word;
            while (getline(sstream, word, ':')) {
                words.push_back(word);
            }
            if (words.size() == 1) tempIdx = 0;

            if (!strcmp(words[tempIdx].c_str(), "posList") || !strcmp(words[tempIdx].c_str(), "pos") ||
                !strcmp(words[tempIdx].c_str(), "pointMember") || !strcmp(words[tempIdx].c_str(), "segments")) {
                return instruction.child_value();
            }
        }
        return "";
    }

    void GM_Object::Save(pugi::xml_node& node)
    {
        auto child = node.append_child("gml:LineString");
        auto temp = child.append_child("gml:posList");
        temp.text().set(Geom[0].c_str());
    }
}
