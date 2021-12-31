#pragma once
#include "LuaScriptingReference.h"

#include <string>

class KRS_LUA_SCRIPT_REFERENCE_API Result_DrawingInstruction
{
public:
	Result_DrawingInstruction() {};
	virtual ~Result_DrawingInstruction() {};

public:
	std::string featureID;
	std::string drawingInstructions;
	std::string observedParameters;
};