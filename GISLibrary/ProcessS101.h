#pragma once

#include "S101LuaScriptingReference.h"

#include <string>
#include <vector>

class S100Layer;
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
	static int ProcessS101_LUA(std::wstring luaRulePath, S100Layer* layer);
	static int ProcessS100_XSLT(std::string inputXmlPath, std::string mainRulePath, std::string outputXmlPath, S100Layer* layer);

	static bool LUA_ParsingDrawingInstructions(std::string featureID, std::vector<std::string> elements, PCOutputSchemaManager* pcm);
	static std::wstring LUA_GetPriority(std::string lua_priority);

	static void InitPortrayal( 
		const char* topLevelRule, 
		S101Cell* cell, 
		FeatureCatalogue* fc, 
		PortrayalCatalogue* pc);

	static void PortrayalSetContextParameter(
		const char* parameterName, 
		const char* parameterValue);
};