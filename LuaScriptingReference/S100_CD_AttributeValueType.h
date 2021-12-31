#pragma once
#include "LuaScriptingReference.h"

#include <map>

enum class KRS_LUA_SCRIPT_REFERENCE_API S100_CD_AttributeValueType
{
	unknown,
	boolean,
	enumeration,
	integer,
	real,
	text,
	date,
	time,
	dateTime,
	URI,
	URL,
	URN,
	S100_CodeList,
	S100_TruncatedDate
};
extern KRS_LUA_SCRIPT_REFERENCE_API std::map<S100_CD_AttributeValueType, std::string> S100_CD_AttributeValueType_Names;