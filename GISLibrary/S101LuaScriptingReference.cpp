#include "stdafx.h"
#include "S101LuaScriptingReference.h"
#include "S101Cell.h"
#include "host_data.h"
#include "host_functions.h"
#include "ProcessS101.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\GeoMetryLibrary\\ENCCommon.h"
#include "..\\LuaScriptingReference\\portrayal_catalog.h"
#include "..\\LuaScriptingReference\\lua_session.h"

#include <iostream>
#include <string>
#include <fstream>

std::string unknown_attribute = "";

S101LuaScriptingReference::S101LuaScriptingReference()
{
	m_lua_session = NULL;
}

S101LuaScriptingReference::~S101LuaScriptingReference()
{
	if(m_lua_session)
		delete m_lua_session;
	m_lua_session = NULL;
}

namespace KRS_LUA_SCRIPT_REFERENCE
{
	void PortrayalSetContextParameter(const char*  parameterName, const char*  parameterValue)
	{
		ProcessS101::theInstance.m_lua_session->call("PortrayalSetContextParameter", { parameterName, parameterValue});
	}
		
	void RemoveResultDrawingInstructions()
	{
		resultDrawingInstructions.clear();

		if (ProcessS101::theInstance.m_lua_session)
		{
			delete ProcessS101::theInstance.m_lua_session;
		}

		ProcessS101::theInstance.m_lua_session = nullptr;
		
		hd_delete();
	}

	void SaveDrawingInstructions(std::string path)
	{
		std::ofstream file;
		file.open(path);

		for (
			auto i = resultDrawingInstructions.begin();
			i != resultDrawingInstructions.end();
			i++)
		{
			file << i->featureID << std::endl;
			file << i->drawingInstructions << std::endl;
			file << i->observedParameters << std::endl;
		}

		file.close();
	}

	std::list<Result_DrawingInstruction>* GetResultDrawingInstructions()
	{ 
		ProcessS101::theInstance.m_lua_session->call("PortrayalMain");

		return &resultDrawingInstructions;
	}
}