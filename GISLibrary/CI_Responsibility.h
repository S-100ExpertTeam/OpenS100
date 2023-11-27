#pragma once
#include <memory>
#include <vector>
#include <string>
#include "CI_RoleCode.h"
#include "CI_Party.h"
#include "pugiSupportClass.h"


using namespace std;

namespace S100 {
	class CI_Responsibility
	{
	public:
		CI_RoleCode role;
		//EX_Extent extent
		vector<CI_Party> party;

	
		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:role"))
				{
					role = CI_RoleCodeFromString(GetContentNode(instruction, "CI_RoleCode","cit").child_value());
				}
				else if (!strcmp(instructionName, "cit:party"))
				{
					CI_Party pa;
					pa.GetContents(instruction);
					party.push_back(pa);
				}
			}
		}
	};

}