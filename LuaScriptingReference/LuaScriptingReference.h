#pragma once

#ifdef KRS_LUA_SCRIPT_REFERENCE_DLL_EXPORTS
	#define KRS_LUA_SCRIPT_REFERENCE_API __declspec(dllexport)
#else
	#define KRS_LUA_SCRIPT_REFERENCE_API __declspec(dllimport)
#endif