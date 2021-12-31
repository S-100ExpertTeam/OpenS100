#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"
#include "S100_FC_RoleType.h"
#include "S100_FC_Role.h"
#include "S100_FC_InformationAssociation.h"

class InformationBinding;
class S100_FC_InformationType;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_InformationBinding
{
public:
	S100_FC_InformationBinding();
	S100_FC_InformationBinding(rapidxml::xml_node<>* node);
	S100_FC_InformationBinding(InformationBinding* node);

public:
	S100_Multiplicity multiplicity;
	S100_FC_RoleType roleType;
	S100_FC_ItemReference<S100_FC_InformationType> informationType;
	S100_FC_ItemReference<S100_FC_Role> role;
	S100_FC_ItemReference<S100_FC_InformationAssociation> association;
};