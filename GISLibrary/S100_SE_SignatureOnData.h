#pragma once

#include <string>
#include "pugixml.hpp"
#include "S100_SE_DigitalSignature.h"
#include "DataStatus.h"


namespace S100 {
	class S100_SE_SignatureOnData : public S100_SE_DigitalSignature
	{
	public:
		DataStatus DataStatus;

		inline void GetContents(pugi::xml_node& node)
		{
			Value = node.child_value();

			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "id"))
				{
					Id = instruction.child_value();
				}
				else if (!strcmp(instructionName, "certificateRef"))
				{
					CertificateRef = instruction.child_value();
				}
				else if (!strcmp(instructionName, "dataStatus"))
				{
					DataStatus = DataStatusFromString(instruction.child_value());
				}
				
			}
		}
	};
}