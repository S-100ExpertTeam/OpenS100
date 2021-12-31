#pragma once
#include "LuaScriptingReference.h"
#include "spatial_association.h"

#include <optional>
#include <vector>

struct KRS_LUA_SCRIPT_REFERENCE_API surface
{
	surface() = default;

	spatial_association exterior_ring;
	std::optional<std::vector<spatial_association>> interior_rings;
};