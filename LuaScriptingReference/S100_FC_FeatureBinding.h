#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"
#include "S100_Multiplicity.h"
#include "S100_FC_RoleType.h"
#include "S100_FC_Role.h"
#include "S100_FC_FeatureAssociation.h"
#include "S100_FC_FeatureType.h"
#include "S100_FC_ItemReference.h"

class FeatureBinding;
class S100_FC_FeatureType;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_FeatureBinding
{
public:
	S100_FC_FeatureBinding();
	S100_FC_FeatureBinding(rapidxml::xml_node<>* node);
	S100_FC_FeatureBinding(FeatureBinding* node);

public:
	S100_Multiplicity multiplicity;
	S100_FC_RoleType roleType;
	S100_FC_ItemReference<S100_FC_FeatureType> featureType;
	S100_FC_ItemReference<S100_FC_Role> role;
	S100_FC_ItemReference<S100_FC_FeatureAssociation> association;
};