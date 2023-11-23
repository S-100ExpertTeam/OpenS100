#pragma once
#include "CI_OnLineFunctionCode.h"

#include <string>
#include <memory>
#include <vector>
#include "pugixml.hpp"



using namespace std;

namespace S100
{
	class CI_OnlineResource
	{
	public:
		string Linkage;
		shared_ptr<string> Protocol;
		shared_ptr<string> ApplicationProfile;
		shared_ptr<string> Name;
		shared_ptr<string> Description;
		shared_ptr<CI_OnLineFunctionCode> Function;
		shared_ptr<string> ProtocolRequest;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "cit:linkage"))
				{
					Linkage = instruction.child_value();
				}
				else if (!strcmp(instructionName, "cit:protocol"))
				{
					Protocol = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:applicationProfile"))
				{
					ApplicationProfile = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:name"))
				{
					Name = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:description"))
				{
					Description = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "cit:function"))
				{
					Function = make_shared<CI_OnLineFunctionCode>(CI_OnLineFunctionCodeFromString(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "cit:protocolRequest"))
				{
					ProtocolRequest = make_shared<string>(instruction.child_value());
				}
			}
		}
		
	};
}