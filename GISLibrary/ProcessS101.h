#pragma once

#include "S101LuaScriptingReference.h"

#include <string>
#include <vector>

class S101Layer;
class PCOutputSchemaManager;

class ProcessS101
{
public:
	ProcessS101();
	virtual ~ProcessS101();

public:
	static S101LuaScriptingReference theInstance;
	static std::string g_unknown_attribute_value;

public:
	static int ProcessS101_LUA(std::wstring luaRulePath, S101Layer* layer);

	static bool LUA_ParsingDrawingInstructions(std::string featureID, std::vector<std::string> elements, PCOutputSchemaManager* pcm);
	static std::wstring LUA_GetPriority(std::string lua_priority);

	static void InitPortrayal(
		const char* featureCatalog, 
		const char* topLevelRule, 
		S101Cell* cell, 
		FeatureCatalogue* fc);

	static void PortrayalSetContextParameter(
		const char* parameterName, 
		const char* parameterValue);
};