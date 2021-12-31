
#include "stdafx.h"
#include <iostream>

#include "lsr.h"

using namespace std;

void check_status(int status)
{
	if (status != 0 && status != LUA_YIELD)
	{
		const char *message = lua_tostring(l, -1);

		cerr << message << "\n";

		exit(3);
	}
}

void set_field_string(string table_name, string field_name, string value)
{
	lua_getglobal(l, table_name.c_str());

	if (lua_type(l, -1) != LUA_TTABLE)
	{
		cerr << "Attempt to set field on non-table global " << table_name << ".\n";

		exit(4);
	}

	lua_pushstring(l, value.c_str());

	lua_setfield(l, -2, field_name.c_str());
}

void get_function(string function_name)
{
	lua_getglobal(l, function_name.c_str());

	if (lua_type(l, -1) != LUA_TFUNCTION)
	{
		cerr << "Attempt to call a non-function  " << function_name << ".\n";

		exit(4);
	}
}

string call_string(int num_params)
{
	lua_pcall(l, num_params, 1, 0);

	string result(lua_tostring(l, -1));

	lua_pop(l, 1);

	return result;
}

void push(lua_variant value)
{
	if (holds_alternative<bool>(value))
		lua_pushboolean(l, get<bool>(value));
	else if (holds_alternative<double>(value))
		lua_pushnumber(l, get<double>(value));
	else if (holds_alternative<string>(value))
		lua_pushstring(l, get<string>(value).c_str());
	else
		_ASSERT(false);
}

lua_variant pop()
{
	lua_variant result;

	switch (lua_type(l, -1))
	{
		case LUA_TNIL:     result = lua_variant(); break;
		case LUA_TBOOLEAN: result = lua_variant((bool)lua_toboolean(l, -1)); break;
		case LUA_TNUMBER:  result = lua_variant(lua_tonumber(l, -1)); break;
		case LUA_TSTRING:  result = lua_variant((string)lua_tostring(l, -1)); break;

		default:
			_ASSERT(false);
			result = lua_variant();
	}

	lua_remove(l, -1);

	return result;
}

lua_variant call_variant(string function_name, vector<lua_variant> parameters)
{
	get_function(function_name);

	for (auto parameter : parameters)
		push(parameter);

	lua_pcall(l, parameters.size(), 1, 0);

	return pop();
}

void call_leave(string function_name, vector<lua_variant> parameters)
{
	get_function(function_name);

	for (auto parameter : parameters)
		push(parameter);

	lua_pcall(l, parameters.size(), 1, 0);
}
