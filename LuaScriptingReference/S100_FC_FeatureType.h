#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"
#include "S100_FC_ObjectType.h"
#include "S100_CD_FeatureUseType.h"
#include "S100_FC_SpatialPrimitiveType.h"
#include "S100_FC_FeatureBinding.h"
#include "S100_FC_ItemReference.h"

#include <vector>

class FeatureType;
class S100_FC_FeatureBinding;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_FeatureType : public S100_FC_ObjectType
{
public:
	S100_FC_FeatureType();
	S100_FC_FeatureType(rapidxml::xml_node<>* node);
	S100_FC_FeatureType(FeatureType* node);

public:
	S100_CD_FeatureUseType featureUseType;
	std::vector<S100_FC_SpatialPrimitiveType> permittedPrimitives;
	std::vector<S100_FC_FeatureBinding> featureBinding;
	S100_FC_ItemReference<S100_FC_FeatureType> superType;
	std::vector<S100_FC_ItemReference<S100_FC_FeatureType>> subType;
};