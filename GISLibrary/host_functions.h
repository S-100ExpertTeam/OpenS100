#pragma once

#include "..\\LuaScriptingReference\\Result_DrawingInstruction.h"

#include <list>

struct lua_State;

class S100_FC;
class S101Cell;

extern std::list<Result_DrawingInstruction> resultDrawingInstructions;

int HostGetFeatureIDs(lua_State *l);
int HostFeatureGetCode(lua_State *l);
int HostGetInformationTypeIDs(lua_State *l);
int HostInformationTypeGetCode(lua_State *l);
int HostFeatureGetSimpleAttribute(lua_State *l);
int HostFeatureGetComplexAttributeCount(lua_State *l);
int HostFeatureGetSpatialAssociations(lua_State *l);
int HostFeatureGetAssociatedFeatureIDs(lua_State *l);
int HostFeatureGetAssociatedInformationIDs(lua_State *l);
int HostGetSpatialIDs(lua_State *l);
int HostGetSpatial(lua_State *l);
int HostSpatialGetAssociatedInformationIDs(lua_State *l);
int HostSpatialGetAssociatedFeatureIDs(lua_State *l);
int HostInformationTypeGetSimpleAttribute(lua_State *l);
int HostInformationTypeGetComplexAttributeCount(lua_State *l);

// Type Information Access Functions
int HostGetFeatureTypeCodes(lua_State *l);
int HostGetInformationTypeCodes(lua_State *l);
int HostGetSimpleAttributeTypeCodes(lua_State *l);
int HostGetComplexAttributeTypeCodes(lua_State *l);
int HostGetRoleTypeCodes(lua_State *l);
int HostGetInformationAssociationTypeCodes(lua_State *l);
int HostGetFeatureAssociationTypeCodes(lua_State *l);
int HostGetFeatureTypeInfo(lua_State *l);
int HostGetInformationTypeInfo(lua_State *l);
int HostGetSimpleAttributeTypeInfo(lua_State *l);
int HostGetComplexAttributeTypeInfo(lua_State *l);

// Spatial Operations Functions
int HostSpatialRelate(lua_State *l);

// Debugger Support Functions
int HostDebuggerEntry(lua_State *l);

// Portrayal Functions
int HostPortrayalEmit(lua_State *l);