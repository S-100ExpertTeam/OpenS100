#pragma once
#include "S100_IC_Feature.h"
#include "S100_IC_DrawingInstruction.h"
#include "XML_Collection.h"

namespace S100 {
	class S100_IC_SuppressedFeatureLayer : public xmlParseable
	{
	public:
		std::string Identifier;
		std::string FeatureCode;
		dataProduct Product;
		std::vector<S100_IC_Feature> FeatureRef;
		XmlCollection<S100_IC_DrawingInstruction> DrawingInstructionRef;


		inline void GetContents(pugi::xml_node& node) override
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "identifier"))
				{
					Identifier = instruction.child_value();
				}
				else if (!strcmp(instructionName, "featureCode"))
				{
					FeatureCode = instruction.child_value();
				}
				else if (!strcmp(instructionName, "product"))
				{
					Product = instruction.child_value();
				}
				else if (!strcmp(instructionName, "featureRef"))
				{
					S100_IC_Feature fe;
					fe.GetContents(instruction);
					FeatureRef.push_back(fe);
				}
				else if (!strcmp(instructionName, "drawingInstructionRef"))
				{
					S100_IC_DrawingInstruction fe;
					fe.GetContents(instruction);
					DrawingInstructionRef.push_back(fe);
				}
				else
				{
					std::wstring content = pugi::as_wide(instructionName);
					content.append(L"is another data");
				}
			}
		}
	};
}

