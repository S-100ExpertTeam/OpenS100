#include "stdafx.h"
#include "stdio.h"
#include "host_functions.h"
#include "host_data.h"
#include "ProcessS101.h"

#include "..\\LuaScriptingReference\\lua_session.h"
#include "..\\LuaScriptingReference\\lua_functions.h"
#include "..\\LuaScriptingReference\\Result_DrawingInstruction.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\FeatureCatalog\\FeatureTypes.h"

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <afxcmn.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib" )

std::list<Result_DrawingInstruction> resultDrawingInstructions;

int HostGetFeatureIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	ls->push(hd_get_feature_ids());

	return 1;
}

int HostFeatureGetCode(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto featureID = ls->pop<std::string>();

	ls->push(hd_get_feature_code(featureID));

	return 1;
}

int HostGetInformationTypeIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	ls->push(hd_get_information_type_ids());

	return 1;
}

int HostInformationTypeGetCode(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto informationTypeID = ls->pop<std::string>();

	ls->push(hd_get_information_type_code(informationTypeID));

	return 1;
}

int HostFeatureGetSimpleAttribute(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto attributeCode = ls->pop<std::string>();
	auto path = ls->pop<std::string>();
	auto featureID = ls->pop<std::string>();

	auto values = hd_get_feature_simple_attribute_values(featureID, path, attributeCode);

	ls->push(values);

	return 1;
}

int HostFeatureGetComplexAttributeCount(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto attributeCode = ls->pop<std::string>();
	auto path = ls->pop<std::string>();
	auto featureID = ls->pop<std::string>();

	auto count = hd_get_feature_complex_attribute_count(featureID, path, attributeCode);

	ls->push(count);

	return 1;
}

// SpatialAssociation[] HostFeatureGetSpatialAssociations(string featureID)
int HostFeatureGetSpatialAssociations(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto featureID = ls->pop<std::string>();

	auto sas = hd_get_feature_spatial_associations(featureID);

	std::vector<lua_ref_ptr> sas_refs;

	for (auto sa : sas)
		sas_refs.push_back(CreateSpatialAssociation(ls, &sa));

	ls->push(sas_refs);

	return 1;
}

int HostFeatureGetAssociatedFeatureIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	assert(false); // TODO: Implement.

	return 0;
}

int HostFeatureGetAssociatedInformationIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto roleCode = ls->pop<std::string>();
	auto associationCode = ls->pop<std::string>();
	auto featureID = ls->pop<std::string>();

	ls->push(hd_get_feature_associated_information_ids(featureID, associationCode, roleCode));

	return 1;
}

int HostGetSpatialIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	assert(false); // TODO: Implement.

	return 0;
}

// Spatial HostGetSpatial(string spatialID)
int HostGetSpatial(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto spatialID = ls->pop<std::string>();

	switch (hd_get_spatial_type(spatialID))
	{
	case spatial_type::point:
		ls->push(CreatePoint(ls, hd_get_point(spatialID)));
		break;

	case spatial_type::multi_point:
		ls->push(CreateMultiPoint(ls, hd_get_multipoint(spatialID)));
		break;

	case spatial_type::curve:
		ls->push(CreateCurve(ls, hd_get_curve(spatialID)));
		break;

	case spatial_type::composite_curve:
		ls->push(CreateCompositeCurve(ls, hd_get_composite_curve(spatialID)));
		break;

	case spatial_type::surface:
		ls->push(CreateSurface(ls, hd_get_surface(spatialID)));
		break;

	default:
		// Not implemented.
		assert(false);
		return 0;
	}

	return 1;
}

int HostSpatialGetAssociatedInformationIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto roleCode = ls->pop<std::string>();
	auto associationCode = ls->pop<std::string>();
	auto spatialID = ls->pop<std::string>();

	ls->push(hd_get_spatial_associated_information_ids(spatialID, associationCode, roleCode));

	return 1;
}

int HostSpatialGetAssociatedFeatureIDs(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto spatialID = ls->pop<std::string>();

	auto feature_ids = hd_get_spatial_associated_feature_ids(spatialID);

	if (feature_ids.empty())
		return 0;

	ls->push(feature_ids);

	return 1;
}

int HostInformationTypeGetSimpleAttribute(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto attributeCode = ls->pop<std::string>();
	auto path = ls->pop<std::string>();
	auto informationTypeID = ls->pop<std::string>();

	auto values = hd_get_information_type_simple_attribute_values(informationTypeID, path, attributeCode);

	ls->push(values);

	return 1;
}

int HostInformationTypeGetComplexAttributeCount(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto attributeCode = ls->pop<std::string>();
	auto path = ls->pop<std::string>();
	auto informationTypeID = ls->pop<std::string>();

	auto count = hd_get_information_type_complex_attribute_count(informationTypeID, path, attributeCode);

	ls->push(count);

	return 1;
}

//
// Type Information Access Functions
//

int HostGetFeatureTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	/*
	-		a	{ size=184 }	std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >
		[capacity]	211	int
+		[allocator]	allocator	std::_Compressed_pair<std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,1>
+		[0]	"AdministrationArea"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[1]	"AirportAirfield"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[2]	"AnchorBerth"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[3]	"AnchorageArea"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[4]	"ArchipelagicSeaLane"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[5]	"ArchipelagicSeaLaneArea"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[6]	"ArchipelagicSeaLaneAxis"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[7]	"BeaconCardinal"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[8]	"BeaconIsolatedDanger"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
	*/

	auto feature_types = pTheFC->GetFeatureTypesPointer().GetFeatureTypePointer();

	std::vector<std::string> ret_feature_codes;

	for (auto iit = feature_types.begin(); iit != feature_types.end(); iit++)
	{
		std::wstring name = iit->first;

		std::string ret = std::string(name.begin(), name.end());

		ret_feature_codes.push_back(ret);
	}

	ls->push(ret_feature_codes);

	return 1;
}

int HostGetInformationTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	/*
	-		a	{ size=5 }	std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >
		[capacity]	6	int
+		[allocator]	allocator	std::_Compressed_pair<std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,1>
+		[0]	"ContactDetails"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[1]	"NauticalInformation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[2]	"NonStandardWorkingDay"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[3]	"ServiceHours"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[4]	"SpatialQuality"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
	*/

	auto information_types = pTheFC->GetInformationTypesPointer().GetInformationTypePointer();

	std::vector<std::string> ret_information_codes;

	for (auto iit = information_types.begin(); iit != information_types.end(); iit++)
	{
		std::wstring name = iit->first;

		std::string ret = std::string(name.begin(), name.end());

		ret_information_codes.push_back(ret);
	}

	ls->push(ret_information_codes);

	return 1;
}

int HostGetSimpleAttributeTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto simpleattribute_types = pTheFC->GetSimpleAttributesPointer().GetSimpleAttributePointer();
	std::vector<std::string> ret_simpleattribute_codes;
	for (auto iit = simpleattribute_types.begin(); iit != simpleattribute_types.end(); iit++)
	{
		std::wstring name = iit->first;

		std::string ret = std::string(name.begin(), name.end());

		ret_simpleattribute_codes.push_back(ret);
	}

	ls->push(ret_simpleattribute_codes);

	return 1;
}

// string[] HostGetComplexAttributeTypeCodes()
int HostGetComplexAttributeTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);
	/*

		-		a	{ size=41 }	std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >
		[capacity]	42	int
+		[allocator]	allocator	std::_Compressed_pair<std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,1>
+		[0]	"directionalCharacter"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[1]	"featureName"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[2]	"featuresDetected"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[3]	"fixedDateRange"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[4]	"frequencyPair"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[5]	"horizontalClearanceFixed"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[6]	"horizontalClearanceOpen"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
	*/
	auto complexattribute_types = pTheFC->GetComplexAttributesPointer().GetComplexAttributePointer();

	std::vector<std::string> ret_complexattribute_codes;

	for (auto iit = complexattribute_types.begin(); iit != complexattribute_types.end(); iit++)
	{
		std::wstring name = iit->first;
		std::string ret = std::string(name.begin(), name.end());
		ret_complexattribute_codes.push_back(ret);
	}

	ls->push(ret_complexattribute_codes);

	return 1;
}

int HostGetRoleTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);
	/*

	-		a	{ size=13 }	std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >
		[capacity]	13	int
+		[allocator]	allocator	std::_Compressed_pair<std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,1>
+		[0]	"auxiliaryTo"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[1]	"componentOf"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[2]	"consistsOf"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[3]	"definedFor"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[4]	"defines"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[5]	"hasAuxiliary"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
	*/
	auto role_types = pTheFC->GetRolesPointer().GetRolePointer();
	std::vector<std::string> ret_role_codes;
	for (auto iit = role_types.begin(); iit != role_types.end(); iit++)
	{
		Role* role = iit->second;
		std::wstring name = role->GetCodeAsWString();

		std::string ret = std::string(name.begin(), name.end());

		ret_role_codes.push_back(ret);
		sort(ret_role_codes.begin(), ret_role_codes.end());
	}

	ls->push(ret_role_codes);

	return 1;
}

int HostGetInformationAssociationTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	/*
	-		a	{ size=2 }	std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >
		[capacity]	2	int
+		[allocator]	allocator	std::_Compressed_pair<std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,1>
+		[0]	"AdditionalInformation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[1]	"SpatialAssociation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
	*/

	auto information_association_types = pTheFC->GetInformationAssociationsPointer().GetInformationAssociationPointer();
	std::vector<std::string> ret_information_association_codes;
	for (auto iit = information_association_types.begin(); iit != information_association_types.end(); iit++)
	{
		std::wstring name = iit->first;

		std::string ret = std::string(name.begin(), name.end());

		ret_information_association_codes.push_back(ret);
	}

	ls->push(ret_information_association_codes);

	return 1;
}

int HostGetFeatureAssociationTypeCodes(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	/*
	-		a	{ size=16 }	std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >
		[capacity]	19	int
+		[allocator]	allocator	std::_Compressed_pair<std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,1>
+		[0]	"ASLAggregation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[1]	"AidsToNavigationAssociation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[2]	"BridgeAggregation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[3]	"CautionAreaAssociation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
+		[4]	"DeepWaterRouteAggregation"	std::basic_string<char,std::char_traits<char>,std::allocator<char> >
	*/

	auto feature_association_types = pTheFC->GetFeatureAssociationsPointer().GetFeatureAssociationPointer();
	std::vector<std::string> ret_feature_association_codes;
	for (auto iit = feature_association_types.begin(); iit != feature_association_types.end(); iit++)
	{
		std::wstring name = iit->first;
		std::string ret = std::string(name.begin(), name.end());
		ret_feature_association_codes.push_back(ret);
	}

	ls->push(ret_feature_association_codes);

	return 1;
}

int HostGetFeatureTypeInfo(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto featureCode = ls->pop<std::string>();
	std::wstring wfeatureCode = std::wstring(featureCode.begin(), featureCode.end());
	FeatureType* ft = pTheFC->GetFeatureType(wfeatureCode);
	ls->push(CreateFeatureType(ls, ft));

	return 1;
}

int HostGetInformationTypeInfo(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto informationCode = ls->pop<std::string>();
	std::wstring winformationCode = std::wstring(informationCode.begin(), informationCode.end());
	auto it = pTheFC->GetInformationType(winformationCode);
	ls->push(CreateInformationType(ls, it));

	return 1;
}

int HostGetSimpleAttributeTypeInfo(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto attributeCode = ls->pop<std::string>();
	std::wstring wsimpleattributeCode = std::wstring(attributeCode.begin(), attributeCode.end());
	auto attr = pTheFC->GetSimpleAttribute(wsimpleattributeCode);
	ls->push(CreateSimpleAttribute(ls, attr));

	return 1;
}

int HostGetComplexAttributeTypeInfo(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto attributeCode = ls->pop<std::string>();
	std::wstring wattributeCode = std::wstring(attributeCode.begin(), attributeCode.end());
	auto attr = pTheFC->GetComplexAttribute(wattributeCode);
	ls->push(CreateComplexAttribute(ls, attr));

	return 1;
}

//
// Spatial Operations Functions
//

int HostSpatialRelate(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	assert(false); // TODO: Implement.

	return 0;
}

//
// Debugger Support Functions
//

int HostDebuggerEntry(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto command = ls->peek<std::string>(1);

	if (command == "trace")
	{
		auto text = ls->peek<std::string>(2);
		std::cout << '\t' << text << std::endl;
		OutputDebugStringA(text.c_str());
	}
	else
	{
		auto text = ls->peek<std::string>(2);
		std::cout << '\t' << text << std::endl;
	}

	return 0;
}

int HostPortrayalEmit(lua_State *l)
{
	auto ls = lua_session::get_session(l);

	auto observedParameters = ls->pop<std::string>();
	auto drawingInstructions = ls->pop<std::string>();
	auto featureID = ls->pop<std::string>();

	Result_DrawingInstruction rdi;

	rdi.featureID = featureID;
	rdi.observedParameters = observedParameters;
	rdi.drawingInstructions = drawingInstructions;

	int id = std::stoi(rdi.featureID.c_str());

	resultDrawingInstructions.push_back(rdi);

	ls->push(true);

	return 1;
}