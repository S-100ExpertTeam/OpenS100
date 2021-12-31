#pragma once
#include "LuaScriptingReference.h"

#include <map>

enum class KRS_LUA_SCRIPT_REFERENCE_API S100_CD_QuantitySpecification
{
	unknown,
	angularVelocity,
	area,
	density,
	duration,
	frequency,
	length,
	planeAngle,
	power,
	pressure,
	salinity,
	speed,
	temperature,
	volume,
	weight,
	otherQuantity
};

extern KRS_LUA_SCRIPT_REFERENCE_API std::map<S100_CD_QuantitySpecification, std::string> S100_CD_QuantitySpecification_Names;