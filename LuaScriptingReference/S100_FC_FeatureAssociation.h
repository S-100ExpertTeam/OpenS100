#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_NamedType.h"
#include "S100_FC_Role.h"

#include "rapidxml.hpp"
#include <vector>

class FeatureAssociation;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_FeatureAssociation : public S100_FC_NamedType
{
public:
	S100_FC_FeatureAssociation();
	S100_FC_FeatureAssociation(rapidxml::xml_node<>* node);
	S100_FC_FeatureAssociation(FeatureAssociation* node);

public:
	std::vector<S100_FC_ItemReference<S100_FC_Role>> role;
	S100_FC_ItemReference<S100_FC_FeatureAssociation> superType;
	std::vector<S100_FC_ItemReference<S100_FC_FeatureAssociation>> subType;
};