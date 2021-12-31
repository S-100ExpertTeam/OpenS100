#pragma once

#include "..\\LuaScriptingReference\\Result_DrawingInstruction.h"

#include <list>

struct lua_State;
class S100_FC;
class S101Cell;

//
// Data Access Functions
//

extern std::list<Result_DrawingInstruction> resultDrawingInstructions;
extern DWORD HostGetFeatureIDs_speed;
extern DWORD HostFeatureGetCode_speed;
extern DWORD HostGetInformationTypeIDs_speed;
extern DWORD HostInformationTypeGetCode_speed;
extern DWORD HostFeatureGetSimpleAttribute_speed;
extern DWORD HostFeatureGetComplexAttributeCount_speed;
extern DWORD HostFeatureGetSpatialAssociations_speed;
extern DWORD HostFeatureGetAssociatedFeatureIDs_speed;
extern DWORD HostFeatureGetAssociatedInformationIDs_speed;
extern DWORD HostGetSpatialIDs_speed;
extern DWORD HostGetSpatial_speed;
extern DWORD HostSpatialGetAssociatedInformationIDs_speed;
extern DWORD HostSpatialGetAssociatedFeatureIDs_speed;
extern DWORD HostInformationTypeGetSimpleAttribute_speed;
extern DWORD HostInformationTypeGetComplexAttributeCount_speed;
extern DWORD HostGetFeatureTypeCodes_speed;
extern DWORD HostGetInformationTypeCodes_speed;
extern DWORD HostGetSimpleAttributeTypeCodes_speed;
extern DWORD HostGetComplexAttributeTypeCodes_speed;
extern DWORD HostGetRoleTypeCodes_speed;
extern DWORD HostGetInformationAssociationTypeCodes_speed;
extern DWORD HostGetFeatureAssociationTypeCodes_speed;
extern DWORD HostGetFeatureTypeInfo_speed;
extern DWORD HostGetInformationTypeInfo_speed;
extern DWORD HostGetSimpleAttributeTypeInfo_speed;
extern DWORD HostGetComplexAttributeTypeInfo_speed;
extern DWORD HostSpatialRelate_speed;
extern DWORD HostDebuggerEntry_speed;
extern DWORD HostPortrayalEmit_speed;

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

//
// Type Information Access Functions
//

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

//
// Spatial Operations Functions
//

int HostSpatialRelate(lua_State *l);

//
// Debugger Support Functions
//

int HostDebuggerEntry(lua_State *l);

//
// Portrayal Functions
//

int HostPortrayalEmit(lua_State *l);