#pragma once

#include "LuaScriptingReference.h"
#include "lua_session.h"
#include "spatial_association.h"
#include "point.h"
#include "multipoint.h"
#include "curve_segment.h"
#include "curve.h"
#include "composite_curve.h"
#include "surface.h"

#include "..\\FeatureCatalog\\Item.h"
#include "..\\FeatureCatalog\\Attribute.h"
#include "..\\FeatureCatalog\\ListedValue.h"
#include "..\\FeatureCatalog\\SimpleAttribute.h"
#include "..\\FeatureCatalog\\AttributeBinding.h"
#include "..\\FeatureCatalog\\ComplexAttribute.h"
#include "..\\FeatureCatalog\\Role.h"
#include "..\\FeatureCatalog\\InformationAssociation.h"
#include "..\\FeatureCatalog\\FeatureAssociation.h"
#include "..\\FeatureCatalog\\InformationType.h"
#include "..\\FeatureCatalog\\FeatureBinding.h"
#include "..\\FeatureCatalog\\FeatureType.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"

class S100_FC_Item;
class S100_FC_NamedType;
class S100_FC_ObjectType;
class S100_FC_ListedValue;
class S100_FC_AttributeBinding;
class S100_FC_InformationBinding;
class S100_FC_FeatureBinding;

//extern S100_FC theFC;
extern KRS_LUA_SCRIPT_REFERENCE_API FeatureCatalogue *pTheFC;

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateSpatialAssociation(lua_session *ls, spatial_association *spatial_association);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreatePoint(lua_session *ls, point point);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateMultiPoint(lua_session *ls, multipoint multipoint);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateCurveSegment(lua_session *ls, curve_segment curve_segment);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateCurve(lua_session *ls, curve curve);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateCompositeCurve(lua_session *ls, composite_curve composite_curve);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateSurface(lua_session *ls, surface surface);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateArcByCenterPoint(lua_session *ls, spatial_association center_point, double radius, double start_angle, double angular_distance);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateCircleByCenterPoint(lua_session *ls, spatial_association center_point, double radius, double start_angle, double angular_distance);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateItem(lua_session *ls, Item *item);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateNamedType(lua_session *ls, NamedType *named_type);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateObjectType(lua_session *ls, S100ObjectType *object_type);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateInformationType(lua_session *ls, InformationType *information_type);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateFeatureType(lua_session *ls, FeatureType *feature_type);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateInformationAssociation(lua_session *ls, InformationAssociation *information_association);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateFeatureAssociation(lua_session *ls, FeatureAssociation *feature_association);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateRole(lua_session *ls, Role *role);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateAttributeConstraints(lua_session *ls, AttributeConstraints *attribute_constraints);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateSimpleAttribute(lua_session *ls, SimpleAttribute *simple_attribute);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateComplexAttribute(lua_session *ls, ComplexAttribute *complex_attribute);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateListedValue(lua_session *ls, ListedValue *listed_value);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateAttributeBinding(lua_session *ls, AttributeBinding *attribute_binding );

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateInformationBinding(lua_session *ls,  InformationBinding *information_binding);

lua_ref_ptr KRS_LUA_SCRIPT_REFERENCE_API CreateFeatureBinding(lua_session *ls, FeatureBinding *feature_binding);