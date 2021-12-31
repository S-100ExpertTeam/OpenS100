#pragma once
#include "LuaScriptingReference.h"

class lua_session;
class KRS_LUA_SCRIPT_REFERENCE_API lua_reference
{
public:
	lua_reference(lua_session *ls);
	virtual ~lua_reference();

	int get_reference() const;

private:
	lua_session *m_ls;
	int m_reference;
};