#pragma once

#include "../LuaScriptingReference/Result_DrawingInstruction.h"
#include "../LuaScriptingReference/context_parameter.h"

#include <string>
#include <list>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

class lua_session;
class S101Cell;
class FeatureCatalogue;
class PortrayalCatalogue;

// Global containing unknown attribute value.
extern std::string unknown_attribute;

namespace KRS_LUA_SCRIPT_REFERENCE
{
	void PortrayalSetContextParameter(const char* parameterName, const char* parameterValue);
	std::list<Result_DrawingInstruction>* GetResultDrawingInstructions();
	void RemoveResultDrawingInstructions();
	void SaveDrawingInstructions(std::string path);
	std::vector<context_parameter> GetContextParameters(PortrayalCatalogue* pc);
	std::string GetCurrentDateTime();
}

class S101LuaScriptingReference
{
public:
	S101LuaScriptingReference();
	virtual ~S101LuaScriptingReference();

public:
	lua_session* m_lua_session;
};