#include "stdafx.h"
#include "lua_reference.h"
#include "lua_session.h"
#include "../extlibs/Lua/include/lua.h"

lua_reference::lua_reference(lua_session *ls)
{
	m_ls = ls;
	m_reference = luaL_ref(ls->m_l, LUA_REGISTRYINDEX);
}

lua_reference::~lua_reference()
{
	luaL_unref(m_ls->m_l, LUA_REGISTRYINDEX, m_reference);
}

int lua_reference::get_reference() const
{
	return m_reference;
}