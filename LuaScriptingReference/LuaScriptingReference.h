#pragma once

// Export attribute of the Lua scripting module. It is a DLL in the Visual Studio solution;
// a static build (KRS_LUA_SCRIPT_REFERENCE_STATIC) or a non-Windows build needs none.
#if defined(KRS_LUA_SCRIPT_REFERENCE_STATIC) || !defined(_WIN32)
	#define KRS_LUA_SCRIPT_REFERENCE_API
#elif defined(KRS_LUA_SCRIPT_REFERENCE_DLL_EXPORTS)
	#define KRS_LUA_SCRIPT_REFERENCE_API __declspec(dllexport)
#else
	#define KRS_LUA_SCRIPT_REFERENCE_API __declspec(dllimport)
#endif