#pragma once
#include "LuaScriptingReference.h"
#include "luaconf.h"
#include "context_parameter.h"

#include <vector>

void KRS_LUA_SCRIPT_REFERENCE_API pc_init(const char *portrayal_catalog);

std::vector<context_parameter> KRS_LUA_SCRIPT_REFERENCE_API pc_get_context_parameters();

std::vector<std::string> KRS_LUA_SCRIPT_REFERENCE_API pc_get_top_level_rules();

void KRS_LUA_SCRIPT_REFERENCE_API pc_delete();