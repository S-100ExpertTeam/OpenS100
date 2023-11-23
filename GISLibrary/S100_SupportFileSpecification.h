#pragma once
#include "S100_Date.h";

#include <string>
#include <memory>

using namespace std;

namespace S100 {
	class S100_SupportFileSpecification
	{
	public:
		string Name;
		shared_ptr<string> Version;
		shared_ptr<S100_Date> Date;

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "S100XC:name"))
				{
					Name = instruction.child_value();
				}
				else if (!strcmp(instructionName, "S100XC:contact"))
				{
					Version = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:date"))
				{
					Date = make_shared<S100_Date>(S100_Date(instruction.child_value()));
				}
				else
				{
				}
			}
		}
	};
}
