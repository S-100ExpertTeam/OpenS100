#pragma once
#include "..\\LuaScriptingReference\\Result_DrawingInstruction.h"

#include <string>
#include <list>

class lua_session;
class S101Cell;
class FeatureCatalogue;

// Global containing unknown attribute value.
extern std::string unknown_attribute;

namespace KRS_LUA_SCRIPT_REFERENCE
{
	void PortrayalSetContextParameter(const char* parameterName, const char* parameterValue);
	std::list<Result_DrawingInstruction>* GetResultDrawingInstructions();
	void RemoveResultDrawingInstructions();
	void SaveDrawingInstructions(std::string path);
}

class S101LuaScriptingReference
{
public:
	S101LuaScriptingReference();
	virtual ~S101LuaScriptingReference();

public:
	lua_session* m_lua_session;
	S101Cell* m_s101_cell;
	FeatureCatalogue* m_s101_feature_catalogue;
};