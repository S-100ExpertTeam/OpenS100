#pragma once
#include "CI_Contact.h"

#include <string>
#include <memory>
#include <vector>
using namespace std;

namespace S100 {

	class CI_Individual 
	{
	public:
		shared_ptr<string> Name;
		vector<CI_Contact> ContactInfo;
		//PartyIdentifier
		vector<string> PositionName;

		inline void GetContents(pugi::xml_node& node)
		{

			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:name"))
				{
					Name =make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:contactInfo"))
				{
					CI_Contact cc;
					cc.GetContents(instruction);
					ContactInfo.push_back(cc);
				}
				else if (!strcmp(instructionName, "cit:partyIdentifier"))
				{
					//////
				}
				else if (!strcmp(instructionName, "	cit:positionName"))
				{
					PositionName.push_back(instruction.child_value());
				}
			}
		}

	};

}