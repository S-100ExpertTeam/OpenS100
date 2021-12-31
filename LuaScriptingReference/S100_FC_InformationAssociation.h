#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_NamedType.h"
#include "S100_FC_ItemReference.h"
#include "S100_FC_Role.h"
#include "rapidxml.hpp"

#include <vector>

class InformationAssociation;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_InformationAssociation : public S100_FC_NamedType
{

public:
	S100_FC_InformationAssociation();
	S100_FC_InformationAssociation(rapidxml::xml_node<>* node);
	S100_FC_InformationAssociation(InformationAssociation* node);

public:
	std::vector<S100_FC_ItemReference<S100_FC_Role>> role;
	S100_FC_ItemReference<S100_FC_InformationAssociation> superType;
	std::vector<S100_FC_ItemReference<S100_FC_InformationAssociation>> subType;
};