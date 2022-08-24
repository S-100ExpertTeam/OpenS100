#pragma once
#include "LuaScriptingReference.h"
#include "luaconf.h"
#include "context_parameter.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

std::vector<context_parameter> KRS_LUA_SCRIPT_REFERENCE_API pc_get_context_parameters_from_pc(PortrayalCatalogue* pc);