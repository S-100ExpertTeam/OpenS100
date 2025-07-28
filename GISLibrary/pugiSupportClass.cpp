#include "stdafx.h"
#include "pugiSupportClass.h"
#include <sstream>
#include <vector>
#include <string>

namespace S100 {
    bool ExXmlSupport::ParseStr2Bool(std::string value)
    {
        if (value == "true") {
            return  true;
        }
        else {
            return false;
        }
    }

    std::string ExXmlSupport::ParseBool2Str(bool value)
    {
        return value ? "true" : "false";
    }


    bool ExXmlSupport::Compare(std::string str, std::string nodeName, std::string nodeNameSpace)
    {
        std::string nodeNameTemp;

        if (nodeNameSpace == "")
            nodeNameTemp = nodeName;
        else
            nodeNameTemp = nodeNameSpace + ":" + nodeName;


        std::stringstream ss(str);
        std::string item;
        std::vector<std::string> tokens;
        while (std::getline(ss, item, ':')) {
            tokens.push_back(item);
        }
        if (tokens.empty())  return false;

        if (tokens.size() == 1)
        {
            if (nodeNameSpace == "")
            {
                if (str == nodeNameTemp)
                    return true;
                else
                    return false;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (nodeNameSpace == "")
            {
                if (tokens[1] == nodeNameTemp)
                    return true;
                else
                    return false;
            }
            else
            {
                if (str == nodeNameTemp)
                    return true;
                else 
                    return false;
            }
        }
    }

    pugi::xml_node& ExXmlSupport::GetContentSelectNode(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace)
    {
        if (node.empty()) {
            return pugi::xml_node();
        }

        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) {
            const pugi::char_t* instructionName = instruction.name();

            if (Compare(instructionName, nodeName, nodeNameSpace)) {
                return instruction;
            }
        }
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node found = GetContentSelectNode(child, nodeName, nodeNameSpace);
            if (!found.empty()) {
                return found;
            }
        }
        return pugi::xml_node();
    }

    pugi::xml_node& ExXmlSupport::GetContentNode(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace)
    {
        std::string nodeNameTemp;

        if (nodeNameSpace == "")
            nodeNameTemp = nodeName;
        else
            nodeNameTemp = nodeNameSpace + ":" + nodeName;

        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, nodeNameTemp.c_str()))
            {
                return instruction;
            }
        }
    }

    const pugi::char_t* ExXmlSupport::GetContentItem(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace)
    {
        std::string nodeNameTemp;

        if (nodeNameSpace == "")
            nodeNameTemp = nodeName;
        else
            nodeNameTemp = nodeNameSpace + ":" + nodeName;

        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, nodeNameTemp.c_str()))
            {
                return instruction.child_value();
            }
        }

        return nullptr;
    }

    void ExXmlSupport::GetContentItemVector(pugi::xml_node& node, std::string nodeName, std::vector<std::string>& item, std::string nodeNameSpace)
    {
        std::string nodeNameTemp;

        if (nodeNameSpace == "")
            nodeNameTemp = nodeName;
        else
            nodeNameTemp = nodeNameSpace + ":" + nodeName;

        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, nodeNameTemp.c_str()))
            {
                item.push_back(instruction.child_value());
            }
        }
    }
}
