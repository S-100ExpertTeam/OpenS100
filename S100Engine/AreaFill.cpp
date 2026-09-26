#include "stdafx.h"
#include "AreaFill.h"

#include <pugixml.hpp>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>


AreaFill::AreaFill()
{
	v1_x = 0;
	v1_y = 0;
	v2_x = 0;
	v2_y = 0;
}

AreaFill::~AreaFill()
{

}

bool AreaFill::Read(const std::string& path)
{
	return ReadByPugi(path);
}

bool AreaFill::Read(const std::wstring& path)
{
	return ReadByPugi(LibMFCUtil::WStringToString(path));
}

static std::string getAttributeValueStr(const std::string& input, const std::string& attributeName)
{
	std::string attributePattern = attributeName + "=\"";
	size_t startPos = input.find(attributePattern);

	if (startPos == std::string::npos)
	{
		return "";
	}

	startPos += attributePattern.length();
	size_t endPos = input.find("\"", startPos);

	if (endPos == std::string::npos)
	{
		return "";
	}

	return input.substr(startPos, endPos - startPos);
}

bool AreaFill::ReadByPugi(const std::string& path)
{
	try
	{
		pugi::xml_document doc;
		doc.load_file(path.c_str(), pugi::parse_full);

		for (auto child = doc.first_child(); child; child = child.next_sibling())
		{
			std::string name = child.name();
			if (name.find("S100Meta") != std::string::npos)
			{
				_name = getAttributeValueStr(child.value(), "name");
			}
			else if (name.find("symbolFill") != std::string::npos)
			{
				for (auto instruction = child.first_child(); instruction; instruction = instruction.next_sibling())
				{
					auto instructionName = instruction.name();
					if (!strcmp(instructionName, "areaCRS"))
					{
						_areaCRS = instruction.child_value();
					}
					else if (!strcmp(instructionName, "symbol"))
					{
						_symbolReference = instruction.attribute("reference").value();
						offset_x = instruction.child("offset").child("x").text().as_double();
						offset_y = instruction.child("offset").child("y").text().as_double();
					}
					else if (!strcmp(instructionName, "v1"))
					{
						v1_x = std::stof(instruction.child("x").child_value());
						v1_y = std::stof(instruction.child("y").child_value());
					}
					else if (!strcmp(instructionName, "v2"))
					{
						v2_x = std::stof(instruction.child("x").child_value());
						v2_y = std::stof(instruction.child("y").child_value());
					}
				}
			}
		}
	}
	catch (std::exception e)
	{
		return false;
	}
	return true;
}

bool AreaFill::ReadByPugi(const std::wstring& path)
{
	return ReadByPugi(LibMFCUtil::WStringToString(path));
}

// Initialization is performed to use COM objects.
// As the window version version goes up, an expanded initialization function may be used.
bool AreaFill::initializeCOM() {
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif
	return true;
}

std::string AreaFill::getNodeValue(pugi::xml_node node)
{
	return "";
}

std::string AreaFill::getNodeAttribute(pugi::xml_node node)
{
	return "";
}