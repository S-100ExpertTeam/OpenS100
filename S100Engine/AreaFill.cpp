#include "stdafx.h"
#include "AreaFill.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <vector>
#include <string>
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

	//try
	//{
	//	pugi::xml_document doc;
	//	auto result = doc.load_file(path.c_str(), pugi::parse_pi);

	//	if (result)
	//	{
	//		pugi::xml_node root = doc.root();

	//		for (auto child = root.first_child(); child; child = child.next_sibling())
	//		{
	//			auto name = child.name();
	//			if (strcmp(name, "S100Meta") == 0)
	//			{
	//				auto type = child.type();
	//				auto value = child.value();
	//				auto meta = std::string(value);
	//				auto startIndex = meta.find("name=\"");
	//				auto sub = meta.substr(startIndex + 6);
	//				auto endIndex = sub.find("\"");
	//				auto sname = sub.substr(0, endIndex);
	//				_name = pugi::as_wide(sname);
	//				break;
	//			}
	//		}
	//	}

	//	initializeCOM();

	//	IXMLDOMDocumentPtr pDoc = NULL;
	//	VARIANT_BOOL bBool;
	//	IXMLDOMNodePtr pNode;

	//	pDoc.CreateInstance(__uuidof(DOMDocument));
	//	pDoc->load((_variant_t)path.c_str(), &bBool);

	//	IXMLDOMNodeListPtr childs;
	//	pDoc->get_childNodes(&childs);

	//	long length = 0;
	//	childs->get_length(&length);

	//	_areaCRS = getNodeValue(pDoc, L"//symbolFill//areaCRS");
	//	_symbolReference = getNodeAttribute(pDoc, L"//symbolFill//symbol");

	//	v1_x = (float)_wtof(getNodeValue(pDoc, L"//symbolFill//v1//x").c_str());
	//	v1_y = (float)_wtof(getNodeValue(pDoc, L"//symbolFill//v1//y").c_str());
	//	v2_x = (float)_wtof(getNodeValue(pDoc, L"//symbolFill//v2//x").c_str());
	//	v2_y = (float)_wtof(getNodeValue(pDoc, L"//symbolFill//v2//y").c_str());
	//}
	//catch (std::exception ex)
	//{
	//	return false;
	//}
	//return true;
}

bool AreaFill::ReadByPugi(std::wstring path)
{
	try
	{
		initializeCOM();
		pugi::xml_document doc;
		doc.load_file(path.c_str());

		auto list = doc.child("symbolFill");
		for (auto instruction = list.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "areaCRS"))
			{
				_areaCRS = pugi::as_wide(instruction.child_value());
			}
			else if (!strcmp(instructionName, "symbol"))
			{
				_symbolReference = pugi::as_wide(instruction.child_value());
				_name = _symbolReference.substr(0, _symbolReference.size() - 1);
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