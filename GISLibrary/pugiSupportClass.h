#pragma once
#include <string>
#include <pugixml.hpp>
#include <vector>

namespace S100 {

	inline bool ParseStr2Bool(std::string value)
	{
		if (value == "true") {
			return  true;
		}
		else {
			return false;
		}
	}



	inline pugi::xml_node& GetContentNode(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "")
	{
		std::string nodeNameTemp = "";

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

	inline const pugi::char_t* GetContentItem(pugi::xml_node& node, std::string nodeName, std::string nodeNameSpace = "")
	{
		std::string nodeNameTemp = "";

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
	}

	inline void GetContentItemVector(pugi::xml_node& node, std::string nodeName, std::vector<std::string>& item, std::string nodeNameSpace = "")
	{
		std::string nodeNameTemp = "";

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
	