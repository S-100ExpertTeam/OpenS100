#pragma once


#include <memory>
#include <vector>
#include <string>
#include "CI_Contact.h"
#include "CI_Organisation.h"
#include "CI_Individual.h"
using namespace std;

namespace S100 {
	class CI_Party 
	{
		CI_Organisation Organisation;
		CI_Individual Individual;

	public:
		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:CI_Organisation"))
				{
					CI_Organisation or ;
					or .GetContents(instruction);
					Organisation = or ;
				}
				else if (!strcmp(instructionName, "certificateRef"))
				{
					CI_Individual id;
					id.GetContents(instruction);
					Individual = id;
				}
			}
		}

	};

}