#pragma once

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "lua_reference.h"
#include "LuaScriptingReference.h"

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <optional>
#include <memory>
#include <cstddef>
#include <tuple>

typedef std::shared_ptr<const lua_reference> lua_ref_ptr;

typedef std::variant<
	std::nullptr_t, 
	bool, 
	int, 
	double, 
	std::string, 
	const char*, 
	lua_ref_ptr,
	std::vector<int>, 
	std::vector<double>, 
	std::vector<std::string>, 
	std::vector<lua_ref_ptr>,
	std::optional<int>, 
	std::optional<double>, 
	std::optional<std::string>
> lua_variant;

class KRS_LUA_SCRIPT_REFERENCE_API lua_session
{
	friend class lua_reference;

public:
	lua_session();
	lua_session(const lua_session &obj);
 	virtual ~lua_session();
public:
	static lua_session* get_session(lua_State *ls);

	//
	// Miscellaneous
	//

	// Loads a file into the Lua interpreter.
	void load_file(std::string file_name);

	// Registers a Lua-callable C function.
	void register_function(std::string function_name, lua_CFunction function);

	// Get the data type of a Lua global variable.
	int get_global_type(std::string global_name);

	//
	// Calls
	//

	// Puts the requested function on the stack.
	void get_function(std::string function_name);

	// Calls the function leaving the result on the stack.
	void call_leave(std::string function_name, std::vector<lua_variant> parameters = std::vector<lua_variant>());

	// Calls a function that returns the result as a lua_variant.
	lua_variant call(std::string function_name, std::vector<lua_variant> parameters = std::vector<lua_variant>());

	// Calls a function that returns the result as a type T.
	template <typename T> // T is bool, double or string.
	T call(std::string function_name, std::vector<lua_variant> parameters = std::vector<lua_variant>())
	{
		return convert_lua_variant<T>(call(function_name, parameters));
	}

	// Calls the underlying lua_pcall function directly.
	void call_raw(int num_args = 0, int num_results = 0);

	//
	// Stack manipulation
	//

	// Pushes the given value on the stack.
	void push(lua_variant value);

	// Pushes the given vector on the stack as an array.
	void push(std::vector<lua_variant> values);

	// Pops a value from the stack and returns it as a lua_variant.
	lua_variant pop();

	// Pops a value from the stack and returns it as a type T.
	template <typename T> // T is bool, double or string.
	T pop() 
	{
		return convert_lua_variant<T>(pop());
	}

	// Peeks at a value from the stack and returns it as a lua_variant.
	lua_variant peek(int index);

	// Peeks at a value from the stack and returns it as a type T.
	template <typename T> // T is bool, double or string.
	T peek(int index)
	{
		return convert_lua_variant<T>(peek(index));
	}


	//
	// Table manipulation
	//

	// Set the field of the global table to the given value.
	void set_field(std::string table_name, std::string field_name, std::string value);

	//
	// Diagnostics
	//

	// Prints the entries in Lua C API stack on the console.
	void capi_stack_dump();

	// Prints the current Lua call stack on the console.
	void call_stack_trace();

private:
	lua_State *m_l;

	void check_status(int status);

	template <typename T>
	T convert_lua_variant(lua_variant value)
	{
		if (std::holds_alternative<T>(value))
			return std::get<T>(value);
		T temp = T();
		return temp;

		exit(5);
	}

	void specialize();

	static std::map<lua_State*, lua_session*> m_state_session_map;

	template <typename T> void push_vector(std::vector<T> values)
	{
		lua_createtable(m_l, (int)values.size(), 0);

		int i = 0;

		for (T value : values)
		{
			push(value);

			lua_rawseti(m_l, -2, ++i);
		}
	}

	template <typename T> void push_optional(std::optional<T> value)
	{
		if (value.has_value())
			push(value.value());
		else
			lua_pushnil(m_l);
	}
};