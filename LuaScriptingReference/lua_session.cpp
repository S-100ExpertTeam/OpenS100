#include "stdafx.h"
#include "lua_session.h"

#include <assert.h>
#include <iostream>
#include <afxcmn.h>
#include <mmsystem.h>
//#pragma comment(lib, "winmm.lib" )

std::map<lua_State*, lua_session*> lua_session::m_state_session_map;

// Specialize templates here so that any unsupported specializations will result in unresolved external errors.
// This function is never actually called.
void lua_session::specialize()
{
	call<bool>("");
	call<double>("");
	call<std::string>("");
	call<const char*>("");
	call<lua_ref_ptr>("");

	pop<bool>();
	pop<double>();
	pop<std::string>();
	pop<const char*>();
	pop<lua_ref_ptr>();

	peek<bool>(0);
	peek<double>(0);
	peek<std::string>(0);
	peek<const char*>(0);
	peek<lua_ref_ptr>(0);
}

static int atpanic(lua_State *l)
{
	std::cerr << "Lua panic!\n";
	return 2;
}

lua_session::lua_session()
{
	m_l = luaL_newstate();

	luaL_openlibs(m_l);

	lua_atpanic(m_l, atpanic);

	m_state_session_map[m_l] = this;
}

lua_session::lua_session(const lua_session &obj)
{
	m_l = obj.m_l;
}

lua_session::~lua_session()
{
	lua_close(m_l);
}

void lua_session::check_status(int status)
{
	if (status != 0 && status != LUA_YIELD)
	{
		const char *message = lua_tostring(m_l, -1);

		std::cerr << message << "\n";

		//assert(false);

		//exit(3);
		return;
	}
}

lua_session* lua_session::get_session(lua_State *l)
{
	return m_state_session_map[l];
}

//
// Miscellaneous
//
void lua_session::load_file(std::string file_name)
{
	check_status(luaL_loadfile(m_l, file_name.c_str()));

	call_raw();
}

void lua_session::register_function(std::string function_name, lua_CFunction function)
{
	lua_pushcfunction(m_l, function);

	lua_setglobal(m_l, function_name.c_str());
}

int lua_session::get_global_type(std::string global_name)
{
	lua_getglobal(m_l, global_name.c_str());

	int type = lua_type(m_l, -1);

	lua_pop(m_l, 1);

	return type;
}

void lua_session::get_function(std::string function_name)
{
	lua_getglobal(m_l, function_name.c_str());

	if (lua_type(m_l, -1) != LUA_TFUNCTION)
	{
		std::cerr << "Attempt to get a non-function  " << function_name << ".\n";

		assert(false);

		exit(4);
	}
}

void lua_session::call_leave(std::string function_name, std::vector<lua_variant> parameters)
{
	get_function(function_name);

	for (auto parameter : parameters)
		push(parameter);

	check_status(lua_pcall(m_l, (int)parameters.size(), 1, 0));
}

lua_variant lua_session::call(std::string function_name, std::vector<lua_variant> parameters)
{
 	get_function(function_name);

	for (auto parameter : parameters)
		push(parameter);

	check_status(lua_pcall(m_l, (int)parameters.size(), 1, 0));
	return pop();
}

void lua_session::call_raw(int num_args, int num_results)
{
	check_status(lua_pcall(m_l, num_args, num_results, 0));
}

void lua_session::push(lua_variant value)
{
	if (std::holds_alternative<nullptr_t>(value))
		lua_pushnil(m_l);

	else if (std::holds_alternative<bool>(value))
		lua_pushboolean(m_l, std::get<bool>(value));

	else if (std::holds_alternative<int>(value))
		lua_pushnumber(m_l, std::get<int>(value));

	else if (std::holds_alternative<double>(value))
		lua_pushnumber(m_l, std::get<double>(value));

	else if (std::holds_alternative<std::string>(value))
		lua_pushstring(m_l, std::get<std::string>(value).c_str());

	else if (std::holds_alternative<const char *>(value))
		lua_pushstring(m_l, std::get<const char *>(value));

	else if (std::holds_alternative<lua_ref_ptr>(value))
	{
		auto ref = std::get<lua_ref_ptr>(value);

		if (ref == nullptr)
			lua_pushnil(m_l);
		else
			lua_rawgeti(m_l, LUA_REGISTRYINDEX, ref->get_reference());
	}

	else if (std::holds_alternative<std::vector<int>>(value))
		push_vector(std::get<std::vector<int>>(value));

	else if (std::holds_alternative<std::vector<double>>(value))
		push_vector(std::get<std::vector<double>>(value));

	else if (std::holds_alternative<std::vector<std::string>>(value))
		push_vector(std::get<std::vector<std::string>>(value));

	else if (std::holds_alternative<std::vector<lua_ref_ptr>>(value))
		push_vector(std::get<std::vector<lua_ref_ptr>>(value));

	else if (std::holds_alternative<std::optional<int>>(value))
		push_optional(std::get<std::optional<int>>(value));

	else if (std::holds_alternative<std::optional<double>>(value))
		push_optional(std::get<std::optional<double>>(value));

	else if (std::holds_alternative<std::optional<std::string>>(value))
		push_optional(std::get<std::optional<std::string>>(value));

	else
		assert(false);
}

void lua_session::push(std::vector<lua_variant> values)
{
	lua_createtable(m_l, (int)values.size(), 0);

	int i = 0;

	for (auto value : values)
	{
		push(value);

		lua_rawseti(m_l, -2, ++i);
	}
}

lua_variant lua_session::pop()
{
	lua_variant result;

	switch (lua_type(m_l, -1))
	{
		case LUA_TNIL:     result = lua_variant(); break;
		case LUA_TBOOLEAN: result = lua_variant((bool)lua_toboolean(m_l, -1)); break;
		case LUA_TNUMBER:  result = lua_variant(lua_tonumber(m_l, -1)); break;
		case LUA_TSTRING:  
		{
			result = lua_variant((std::string)lua_tostring(m_l, -1));
			break;
		}
		case LUA_TTABLE:
		{
			lua_ref_ptr p(new lua_reference(this));
			return lua_variant(p);
			//return lua_variant();
		}
		case LUA_TFUNCTION:
		{
			lua_ref_ptr p(new lua_reference(this));
			return lua_variant(p);
		}
		default:
			assert(false);
			result = lua_variant();
	}

	lua_remove(m_l, -1);
	return result;
}

lua_variant lua_session::peek(int index)
{
	lua_variant result;

	switch (lua_type(m_l, index))
	{
	case LUA_TNIL:     result = lua_variant(); break;
	case LUA_TBOOLEAN: result = lua_variant((bool)lua_toboolean(m_l, index)); break;
	case LUA_TNUMBER:  result = lua_variant(lua_tonumber(m_l, index)); break;
	case LUA_TSTRING:  result = lua_variant((std::string)lua_tostring(m_l, index)); break;

	default:
		assert(false);
		result = lua_variant();
	}

	return result;
}

//
// Table manipulation
//

void lua_session::set_field(std::string table_name, std::string field_name, std::string value)
{
	lua_getglobal(m_l, table_name.c_str());

	if (lua_type(m_l, -1) != LUA_TTABLE)
	{
		std::cerr << "Attempt to set field on non-table global " << table_name << ".\n";

		exit(4);
	}

	lua_pushstring(m_l, value.c_str());

	lua_setfield(m_l, -2, field_name.c_str());
}

//
// Diagnostics
//

void lua_session::capi_stack_dump()
{
	int top = lua_gettop(m_l);

	for (int i = 1; i <= top; i++)
	{
		int t = lua_type(m_l, i);

		switch (t)
		{
			case LUA_TSTRING:
				std::cout << lua_tostring(m_l, i) << std::endl;
				break;

			case LUA_TBOOLEAN:
				std::cout << (lua_toboolean(m_l, i) ? "true" : "false") << std::endl;
				break;

			case LUA_TNUMBER:
				std::cout << lua_tonumber(m_l, i) << std::endl;
				break;

			default:
				std::cout << lua_typename(m_l, t) << std::endl;
				break;
		}
	}

	std::cout << std::endl;
}

void lua_session::call_stack_trace()
{
	lua_Debug ar;

	int level = 1;

	while (lua_getstack(m_l, level++, &ar) == 1)
	{
		lua_getinfo(m_l, "nSlu", &ar);

		if (ar.name)
			std::cout << '\t' << ar.short_src << " (" << ar.currentline << "): " << ar.name << std::endl;
		else
			std::cout << '\t' << ar.short_src << " (" << ar.currentline << ")" << std::endl;


		int local = 1;
		const char *local_name;

		while (local_name = lua_getlocal(m_l, &ar, local++))
		{
			std::cout << "\t\t" << local_name << "=";

			auto value = this->pop();

			if (std::holds_alternative<nullptr_t>(value))
				std::cout << "nil";

			else if (std::holds_alternative<bool>(value))
				std::cout << (std::get<bool>(value) ? "true" : "false");

			else if (std::holds_alternative<int>(value))
				std::cout << std::get<int>(value);

			else if (std::holds_alternative<double>(value))
				std::cout << std::get<double>(value);

			else if (std::holds_alternative<std::string>(value))
				std::cout << "\"" << std::get<std::string>(value) << "\"";

			else if (std::holds_alternative<const char *>(value))
				std::cout << "\"" << std::get<const char *>(value) << "\"";

			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}
