#include "stdafx.h"
#include "S100_LineStyle.h"
#include "SVG.h"


namespace S100_SVG_D2D1_DLL 
{
	S100_LineStyle::S100_LineStyle()
	{

	}

	S100_LineStyle::~S100_LineStyle()
	{ 

	}

	S100_Line S100_LineStyle::ReadLineStyleByPugi(std::wstring path)
	{
	
		if (!path.empty())
		{
			pugi::xml_document xmldoc;
			pugi::xml_parse_result result = xmldoc.load_file(path.c_str());

			m_filepath = path;
			GetLineStyleInfo(xmldoc);
		}
		return line;
	}

	void S100_LineStyle::GetLineStyleInfo(pugi::xml_node node)
	{
		if (node==nullptr)
			return;

		auto linestyleNode = node.first_child();
		const pugi::char_t* linestyleNodeName = linestyleNode.name();

		if (!strcmp(linestyleNodeName, "lineStyle"))
		{
			for (pugi::xml_node instruction = linestyleNode.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();
				if (!strcmp(instructionName, "pen"))
				{
					GetPen(instruction);
				}
				else if (!strcmp(instructionName, "dash"))
				{
					GetDash(instruction);
				}
				else if (!strcmp(instructionName, "symbol"))
				{
					GetSymbol(instruction);
				}
				else if (!strcmp(instructionName, "intervalLength"))
				{
					auto intervalLengthvalue = pugi::as_wide(instruction.child_value());
					if (intervalLengthvalue!=L"")
					{
						line.length =std::stof(intervalLengthvalue)* factor;
					}
				}
			}
		}
		return;
	}

	void S100_LineStyle::GetPen(pugi::xml_node node)
	{
		if (node==nullptr)
		{
			return;
		}

		auto widthNode = node.first_attribute();
		auto widthName= widthNode.name();

		if (!strcmp(widthName, "width"))
		{
			auto widthNameValue =pugi::as_wide(widthNode.value());

			if (widthNameValue!=L"")
			{
				line.pen.width =std::stof(widthNameValue);
			}
		}

		auto colorNode = node.first_child();
		auto colornodeName = colorNode.name();

		if (!strcmp(colornodeName,"color")) 
		{
			line.pen.col =pugi::as_wide(colorNode.child_value());
		}
	}

	void S100_LineStyle::GetDash(pugi::xml_node node)
	{
		if (node == nullptr)
		{
			return;
		}

		S100_LineDash dash;
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"start"))
			{
				auto startValue =pugi::as_wide(instruction.child_value());
				if (startValue!=L"")
				{
					dash.start = (std::stof(startValue))* factor;
				}
			}

			else if (!strcmp(instructionName, "length"))
			{
				auto lengthValue=pugi::as_wide(instruction.child_value());
				if (lengthValue!=L"") 
				{
					dash.length = (std::stof(lengthValue))*factor;
				}
			}
		}
		line.dash.push_back(dash);
	}

	void S100_LineStyle::GetSymbol(pugi::xml_node node)
	{
		if (node == nullptr)
		{
			return;
		}

		S100_LineSymbol sym;
		pugi::xml_attribute referenceNode = node.first_attribute();
		auto referenceName = referenceNode.name();
		if (!strcmp(referenceName, "reference"))
		{
			sym.reference =pugi::as_wide(referenceNode.value());
		}


		pugi::xml_node positionNode = node.first_child();
		auto positionName = positionNode.name();

		if (!strcmp(positionName, "position"))
		{
			auto positinoValue= pugi::as_wide(positionNode.child_value());
			if (positinoValue!=L"")
			{
				sym.position = std::stof(positinoValue)*factor;
			}
		}

		line.sym.push_back(sym);
	}
}