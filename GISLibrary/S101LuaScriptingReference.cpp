#include "stdafx.h"
#include "S101LuaScriptingReference.h"
#include "S101Cell.h"
#include "host_data.h"
#include "host_functions.h"
#include "ProcessS101.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/Enum_ParameterType.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include "../LuaScriptingReference/lua_session.h"

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
		//ProcessS101::theInstance.m_lua_session->push(hd_get_feature_ids());
		ProcessS101::theInstance.m_lua_session->call("PortrayalMain");

		return &resultDrawingInstructions;
	}

	std::vector<context_parameter> GetContextParameters(PortrayalCatalogue* pc)
	{
		std::vector<context_parameter> result;

		auto context = pc->GetContext();
		auto cnt = context->GetCountOfParameter();

		for (int i = 0; i < cnt; i++)
		{
			auto cp = context->GetContextParameter(i);
			if (cp)
			{
				auto id = cp->GetIdAsString();
				auto enumType = cp->GetType();
				std::string strType;
				auto default_value = cp->GetDefaultAsString();

				if (enumType == ParameterType::Boolean)
				{
					strType = "boolean";
				}
				else if (enumType == ParameterType::Integer)
				{
					strType = "integer";
				}
				else if (enumType == ParameterType::Double)
				{
					strType = "real";
				}
				else if (enumType == ParameterType::String)
				{
					strType = "text";
				}
				else if (enumType == ParameterType::Date)
				{
					strType = "date";
				}

				context_parameter newItem;
				newItem.name = id;
				newItem.type = strType;
				newItem.default_value = default_value;
				result.push_back(newItem);
			}
		}

		return result;
	}
}