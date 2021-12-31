#pragma once

#include "LuaScriptingReference.h"
#include "rapidxml.hpp"
#include "UnlimitedInteger_LUA.h"

class KRS_LUA_SCRIPT_REFERENCE_API S100_Multiplicity
{
public:
	S100_Multiplicity();
	S100_Multiplicity(rapidxml::xml_node<>* node);
	S100_Multiplicity(Multiplicity* node);

public:
	int lower;
	UnlimitedInteger_LUA upper;
};