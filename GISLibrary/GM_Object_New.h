#pragma once
#include <string>
#include <vector>

#include <sstream> 

namespace S100
{
	class GM_Object
	{
	public:
		std::string  Description;
		std::string  DescriptionReference;
		std::string  Identifier;
		std::string  Name;

		std::string  Type;

		std::vector<std::string>  Geom;



		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				std::vector<std::string> words;
				std::stringstream sstream((std::string)instructionName);
				std::string word;
				while (getline(sstream, word, ':'))
				{
					words.push_back(word);
				}

				if (!strcmp(words[1].c_str(), "point"))
				{
					Geom.push_back(GetGeoms(instruction));
				}
				else if (!strcmp(words[1].c_str(), "multiPoint"))
				{
					Geom.push_back(GetGeoms(instruction));
				}
				else if (!strcmp(words[1].c_str(), "lineString"))
				{
					Geom.push_back(GetGeoms(instruction));
				}
				else if (!strcmp(words[1].c_str(), "curve"))
				{
					Geom.push_back(GetGeoms(instruction));
				}
				else if (!strcmp(words[1].c_str(), "multiCurve"))
				{
					Geom.push_back(GetGeoms(instruction));
				}
				else if (!strcmp(words[1].c_str(), "polygon"))
				{
					auto temp = GetContentNode(instruction, "LineString", "gml");
					Geom.push_back(GetGeoms(temp));
				}
				else
				{
				}
			}
		}


		inline std::string GetGeoms(pugi::xml_node& node)
		{

			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();
				
				int tempIdx = 1;
				std::vector<std::string> words;
				std::stringstream sstream((std::string)instructionName);
				std::string word;
				while (getline(sstream, word, ':'))
				{
					words.push_back(word);
				}
				if (words.size() == 1) tempIdx = 0;

				if (!strcmp(words[tempIdx].c_str(), "posList"))
				{
					return instruction.child_value();
				}
				else if (!strcmp(words[tempIdx].c_str(), "pos"))
				{
					return instruction.child_value();
				}
				else if (!strcmp(words[tempIdx].c_str(), "pointMember"))
				{
					return instruction.child_value();
				}
				else if (!strcmp(words[tempIdx].c_str(), "segments"))
				{
					return instruction.child_value();
				}
				else
				{
					return "";
				}
			}
		}
	};
}

//pos
//posList
//pointMember
//segments