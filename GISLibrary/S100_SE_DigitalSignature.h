#pragma once
#include <string>
#include "pugixml.hpp"


using namespace std;


namespace S100 {
	class S100_SE_DigitalSignature
	{
	public :
		string Id;
		string CertificateRef;
		string Value;

		inline void GetContents(pugi::xml_node& node)
		{
			Id = node.attribute("id").as_string();
			CertificateRef = node.attribute("certificateRef").as_string();
			Value = node.child_value();

			/*for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				instruction.
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "id"))
				{
					Id = instruction.child_value();
				}
				else if (!strcmp(instructionName, "certificateRef"))
				{
					CertificateRef = instruction.child_value();
				}
			}*/
		}
	};
}