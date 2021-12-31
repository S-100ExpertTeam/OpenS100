#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include "..\\FeatureCatalog\\Reference.h"

#include <string>

class Reference;
template <class T>
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_ItemReference
{
public:
	S100_FC_ItemReference() {};
	S100_FC_ItemReference(rapidxml::xml_node<>* node) 
	{
		referenceCode = node->first_attribute("ref")->value();
	}

	S100_FC_ItemReference(Reference* node)
	{
		for (auto a : node->attributes)
		{
			if (a.name == L"ref")
			{
				referenceCode = std::string(a.value.begin(), a.value.end());
			}
		}
	}

public:
	std::string referenceCode;
};