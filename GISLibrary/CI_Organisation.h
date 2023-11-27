#pragma once

#include "CI_Contact.h"
#include "CI_Responsibility.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace S100 {
	class CI_Organisation
	{
	public :
		shared_ptr<string> Name;
		vector<CI_Contact> contactInfo;
		//vector<MD_BrowseGraphic> Logo;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:name"))
				{
					Name = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:CI_Contact"))
				{
					CI_Contact cc;
					cc.GetContents(instruction);
					contactInfo.push_back(cc);
				}
			}
		}
	};
}
