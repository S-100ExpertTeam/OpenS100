#pragma once

#include "CI_Telephone.h"
#include "CI_Address.h"
#include "CI_OnlineResource.h"

#include <string>
#include <memory>



using namespace std;

namespace S100
{
    class CI_Contact {
    public:
        vector<CI_Telephone> Phone;
        vector<CI_Address> Address;
		vector<CI_OnlineResource> OnlineResource;

        vector<std::string> HoursOfService;
        shared_ptr<string> ContactInstructions;
        shared_ptr<string> ContactType;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:phone"))
				{
					CI_Telephone tp;
					tp.GetContents(instruction);
					Phone.push_back(tp);
				}
				else if (!strcmp(instructionName, "cit:address"))
				{
					CI_Address ad;
					ad.GetContents(instruction);
					Address.push_back(ad);
				}
				else if (!strcmp(instructionName, "cit:onlineResource"))
				{
					CI_OnlineResource or;
					or.GetContents(instruction);
					OnlineResource.push_back(or);
				}
				else if (!strcmp(instructionName, "cit:hoursOfService"))
				{
					HoursOfService.push_back(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:contactInstructions"))
				{
					ContactInstructions = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:contactType"))
				{
					ContactType = make_shared<string>(instruction.child_value());
				}
			}
		}
    };
}
