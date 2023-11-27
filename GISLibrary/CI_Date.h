#pragma once

#include "CI_DateTypeCode.h"
#include "S100_Date.h"



namespace S100 
{
	class CI_Date
	{
	public:
		S100_Date date;
		CI_DateTypeCode dateType;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();
				if (!strcmp(instructionName, "cit:date"))
				{
					date = S100_Date(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:dateType"))
				{
					dateType = CI_DateTypeCodeFromString(instruction.child_value());
				}
			}
		}
	};
}