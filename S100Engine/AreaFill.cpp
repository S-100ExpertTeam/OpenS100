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

bool AreaFill::Read(std::wstring path)
{
	return ReadByPugi(path);
}

bool AreaFill::ReadByPugi(std::wstring path)
{
	try
	{
		//initializeCOM();
		pugi::xml_document doc;
		auto result = doc.load_file(path.c_str(),pugi::parse_full);

		auto meta = doc.child("S100Meta");
		std::wstring metaValue= pugi::as_wide(meta.value());

		std::wistringstream ss(metaValue);
		std::wstring strBuffer;
		std::vector<std::wstring> v;

		while (getline(ss, strBuffer,L'"'))
		{
			v.push_back(strBuffer);
		}
		_name = v.at(1);
		_exposition = v.at(3);

		auto listXPathNode = doc.select_node("/*[local-name() = 'symbolFill']");
		auto list = listXPathNode.node();
		for (auto instruction = list.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "areaCRS"))
			{
				_areaCRS = pugi::as_wide(instruction.child_value());
			}
			else if (!strcmp(instructionName, "symbol"))
			{
				_symbolReference = pugi::as_wide(instruction.attribute("reference").value());
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
	catch (std::exception e)
	{
		return false;
	}
	catch (std::out_of_range e) {
		return false;
	}
	return true;
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

std::wstring AreaFill::getNodeValue(pugi::xml_node node)
{
	return L"";
}

std::wstring AreaFill::getNodeAttribute(pugi::xml_node node)
{
	return L"";
}