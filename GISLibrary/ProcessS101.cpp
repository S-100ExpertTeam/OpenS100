#include "stdafx.h"
#include "ProcessS101.h"
#include "S100Layer.h"
#include "S101Cell.h"
#include "PCOutputSchemaManager.h"
#include "SENC_DisplayList.h"
#include "host_data.h"
#include "host_functions.h"

#include "../LuaScriptingReference/lua_functions.h"
#include "../LuaScriptingReference/lua_session.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include "../PortrayalCatalogue/S100_ColorFill.h"
#include "../PortrayalCatalogue/S100_AreaFillReference.h"
#include "../PortrayalCatalogue/S100_DisplayList.h"
#include "../PortrayalCatalogue/S100_AreaInstruction.h"
#include "../PortrayalCatalogue/S100_TextInstruction.h"
#include "../PortrayalCatalogue/S100_AlertReference.h"
#include "../PortrayalCatalogue/S100_AugmentedRay.h"
#include "../PortrayalCatalogue/S100_AugmentedPath.h"
#include "../PortrayalCatalogue/S100_LineInstruction.h"
#include "../PortrayalCatalogue/S100_PointInstruction.h"
#include "../PortrayalCatalogue/S100_SymbolFill.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/DisplayFactory.h"
#include "../PortrayalCatalogue/ViewingGroupLayer.h"

#include "../LatLonUtility/LatLonUtility.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <sstream> 

using namespace LatLonUtility;

S101LuaScriptingReference ProcessS101::theInstance;
std::string ProcessS101::g_unknown_attribute_value = "";

ProcessS101::ProcessS101()
{

}

ProcessS101::~ProcessS101()
{

}

void dump_com_error(_com_error& e)
{
	printf("Error\n");
	printf("\a\t Code = %08lx\n", e.Error());
	printf("\a\t Code meaning = %ws", e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	printf("\a\tSource = %s\n", (LPCSTR)bstrSource);
	printf("\a\tDescription = %s\n", (LPCSTR)bstrDescription);
}

int ProcessS101::ProcessS101_LUA(std::wstring luaRulePath, S100Layer* layer)
{
	try {
		auto fc = layer->GetFeatureCatalog();
		auto pc = layer->GetPC();
		if (nullptr == fc || nullptr == pc)
		{
			return 0;
		}

		std::string luaRulePath_string = LibMFCUtil::WStringToString(luaRulePath);

		InitPortrayal(
			luaRulePath_string.c_str(),
			(S101Cell*)layer->m_spatialObject,
			fc,
			pc);

		std::string two_shades = ENCCommon::TWO_SHADES ? "true" : "false";
		std::string national_language = ENCCommon::SHOW_NOBJNM ? "eng" : "kor";
		std::string ignore_scamin = ENCCommon::APPLY_SCALE_MIN ? "false" : "true";
		std::string shallow_pattern = ENCCommon::SHALLOW_PATTERN ? "true" : "false";
		std::string simplified_points = ENCCommon::SIMPLIFIED_POINT_SYMBOL ? "true" : "false";
		std::string show_isolated_dangers_in_shallow_waters = ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER ? "true" : "false";
		std::string plain_boundaries = ENCCommon::SymbolizedAreaBoundary ? "false" : "true";
		std::string safety_depth = std::to_string(ENCCommon::SAFETY_DEPTH);
		std::string sallow_contour = std::to_string(ENCCommon::SHALLOW_CONTOUR);
		std::string safety_contour = std::to_string(ENCCommon::SAFETY_CONTOUR);
		std::string deep_contour = std::to_string(ENCCommon::DEEP_CONTOUR);
		std::string full_sector = ENCCommon::FULL_SECTORS ? "true" : "false";
		std::string radar_overlay = ENCCommon::DISPLAY_PLANE ? "true" : "false";

		auto context = pc->GetContext();
		if (!context) {
			return 0;
		}

		int numContext = context->GetCountOfParameter();
		for (int i = 0; i < numContext; i++) {
			auto contextParameter = context->GetContextParameter(i);
			if (contextParameter) {
				auto name = contextParameter->GetIdAsString();
				auto defaultValue = contextParameter->GetDefaultAsString();
				KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter(name.c_str(), defaultValue.c_str());
			}
		}

		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("SafetyDepth", safety_depth.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("ShallowContour", sallow_contour.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("SafetyContour", safety_contour.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("TwoShades", two_shades.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("DeepContour", deep_contour.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("ShallowPattern", shallow_pattern.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("ShowIsolatedDangersInShallowWaters",
		//	show_isolated_dangers_in_shallow_waters.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("PlainBoundaries", "false");
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("SimplifiedPoints", simplified_points.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("FullSectors", full_sector.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("RadarOverlay", radar_overlay.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("IgnoreScamin", ignore_scamin.c_str());
		//KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("NationalLanguage", national_language.c_str());

		std::list<Result_DrawingInstruction>* drawingInstructionResult = KRS_LUA_SCRIPT_REFERENCE::GetResultDrawingInstructions();

		int t = (int)drawingInstructionResult->size();

		S101Cell* c = (S101Cell*)layer->m_spatialObject;
		c->InitDrawingInstruction();

		for (auto i = drawingInstructionResult->begin(); i != drawingInstructionResult->end(); i++)
		{
			std::string drawingInstructions = i->drawingInstructions;
			std::vector<std::string> di_splited = Split(drawingInstructions, ";");

			c->pcManager->ParsingSENCInstruction(layer, i->featureID, di_splited);
		}

		KRS_LUA_SCRIPT_REFERENCE::SaveDrawingInstructions("..\\TEMP\\drawingCommands.txt");
		KRS_LUA_SCRIPT_REFERENCE::RemoveResultDrawingInstructions();
	}
	catch (_com_error& e)
	{
		dump_com_error(e);
	}
	return 0;
}

int ProcessS101::ProcessS100_XSLT(std::string inputXmlPath, std::string mainRulePath, std::string outputXmlPath, S100Layer* layer)
{
	// Initialize the libraries
	xmlInitParser();
	LIBXML_TEST_VERSION
		xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
	xmlSubstituteEntitiesDefault(1);

	// Load XML and XSL
	xmlDocPtr doc = xmlParseFile(inputXmlPath.c_str());
	xsltStylesheetPtr xslt = xsltParseStylesheetFile((const xmlChar*)mainRulePath.c_str());

	// Transform
	xmlDocPtr result = xsltApplyStylesheet(xslt, doc, nullptr);

	// Output the transformed XML
	FILE* outFile = fopen(outputXmlPath.c_str(), "wb");
	xsltSaveResultToFile(outFile, result, xslt);
	fclose(outFile);

	// Cleanup
	xsltFreeStylesheet(xslt);
	xmlFreeDoc(result);
	xmlFreeDoc(doc);

	xsltCleanupGlobals();
	xmlCleanupParser();

	return 0;
}

bool ProcessS101::LUA_ParsingDrawingInstructions(std::string featureID, std::vector<std::string> elements, PCOutputSchemaManager* pcm)
{
	// Visibility
	std::string v_ViewingGroup;
	std::string v_DisplayPlane;
	std::string v_DrawingPriority;
	std::string v_ScaleMinimum;
	std::string v_ScaleMaximum;

	// Transform
	std::string v_LocalOffset;
	std::string v_LinePlacement;
	std::string v_AreaPlacement;
	std::string v_AreaCRS;
	std::string v_Rotation;
	std::string v_ScaleFactor;

	// Pen Style
	std::string v_PenColor;
	std::string v_PenWidth;

	// Line Style
	std::string v_LineStyle;
	std::string v_LineSymbol;
	std::string v_Dash;

	// Text Style
	std::string v_FontColor;
	std::string v_FontSize = "10";
	std::string v_FontProportion;
	std::string v_FontWeight;
	std::string v_FontSlant;
	std::string v_FontSerifs;
	std::string v_FontUnderline;
	std::string v_FontStrikethrough;
	std::string v_FontUpperline;
	std::string v_FontReference;
	std::string v_TextAlignHorizontal;
	std::string v_TextAlignVertical;
	std::string v_TextVerticalOffset;

	// Colour Override
	std::string v_OverrideColor;
	std::string v_OverrideAll;

	// Geometry
	std::string v_SpatialReference;
	std::string v_AugmentedPoint;
	std::string v_AugmentedRay;
	std::string v_AugmentedPath;
	std::string v_Polyline;
	std::string v_Arc3Points;
	std::string v_ArcByRadius;
	std::string v_Annulus;
	std::string v_ClearAugmented;

	// Coverage
	std::string v_LookupEntry;
	std::string v_NumericAnnotation;
	std::string v_SymbolAnnotation;
	std::string v_CoverageColor;

	std::list<std::string> vl_SpatialReference;

	//S100_LineStyle lineStyle;
	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		std::string element = *i;
		std::vector<std::string> di_splited = Split(element, ":");

		int splitedSize = (int)di_splited.size();

		if (splitedSize == 1)
		{
			// "NullInstruction"
			// "ClearGeometry"
			if (di_splited[0].compare("ClearGeometry") == 0)
			{
				v_AugmentedPoint = "";
				vl_SpatialReference.clear();
				v_AugmentedRay = "";
				v_AugmentedPath = "";
				v_ArcByRadius = "";
			}
		}
		else
		{
			// Splited Size 2
			std::string tag = di_splited[0];
			std::string value = di_splited[1];

			switch (tag.size())
			{
			case 4:
			{
				if (tag == "Dash")
				{
					v_Dash = value;
				}
			}
			break;
			case 8:
			{
				// "FontSize:10"
				if (tag == "FontSize")
					v_FontSize = value;
				// "Rotation:PortrayalCRS,135"
				else if (tag == "Rotation")
					v_Rotation = value;
			}
			break;
			case 9:
			{
				// "ColorFill:DEPVS"
				if (tag == "ColorFill")
				{
					CDisplayFactory::createAreaInstructionA(pcm->displayList,
						featureID,
						v_DrawingPriority,
						v_DisplayPlane,
						v_ViewingGroup,
						v_ScaleMinimum,
						value);
				}
				else if (tag == "LineStyle")
				{
					v_LineStyle = value;
				}
				else if (tag == "FontColor")
					v_FontColor = value;
				else if (tag == "FontSlant")
					v_FontSlant = value;
			}
			break;
			case 11:
			{
				// "LocalOffset:-3.51,3.51"
				if (tag.compare("LocalOffset") == 0)
				{
					v_LocalOffset = value;
				}
				// "ScaleFactor:0.311"
				else if (tag.compare("ScaleFactor") == 0)
				{
					v_ScaleFactor = value;
				}
				// "ArcByRadius:0,0,20,24,199"
				else if (tag.compare("ArcByRadius") == 0)
				{
					v_ArcByRadius = value;
				}
			}
			break;
			case 12:
			{
				// "ScaleMinimum:179999"
				if (tag.compare("ScaleMinimum") == 0)
				{
					v_ScaleMinimum = value;
				}
				// "ViewingGroup:27020"
				else if (tag.compare("ViewingGroup") == 0)
				{
					v_ViewingGroup = value;
				}
				// "DisplayPlane:OverRADAR"
				else if (tag.compare("DisplayPlane") == 0)
				{
					v_DisplayPlane = value;
				}
				// "AugmentedRay:GeographicCRS,24,GeographicCRS,16668"
				else if (tag.compare("AugmentedRay") == 0)
				{
					v_AugmentedRay = value;

					v_AugmentedPath = "";
				}
			}
			break;
			case 13:
			{
				if (tag.compare("LinePlacement") == 0)
				{
					v_LinePlacement = value;
				}
				else if (tag.compare("AugmentedPath") == 0)
				{
					v_AugmentedPath = value;
					v_AugmentedRay = "";
				}
			}
			break;
			case 14:
			{
				if (tag.compare("AugmentedPoint") == 0)
				{
					v_AugmentedPoint = value;
				}
			}
			break;
			case 15:
			{
				// "DrawingPriority:8"
				if (tag.compare("DrawingPriority") == 0)
				{
					v_DrawingPriority = value;
				}
				// "TextInstruction:bn Alligator River Light 16,21,8"
				else if (tag.compare("TextInstruction") == 0)
				{
					CDisplayFactory::createTextInstruction(pcm->displayList,
						featureID,
						v_DrawingPriority,
						v_DisplayPlane,
						v_ViewingGroup,
						v_ScaleMinimum,
						v_TextAlignVertical,
						v_TextAlignHorizontal,
						v_LocalOffset,
						value,
						v_FontSize,
						v_FontSlant,
						v_FontColor);
				}
				else if (tag.compare("LineInstruction") == 0)
				{
					if (!v_AugmentedRay.empty())
					{
						CDisplayFactory::createAugmentedRay(pcm->displayList,
							featureID,
							v_DrawingPriority,
							v_DisplayPlane,
							v_ViewingGroup,
							v_ScaleMinimum,
							v_LineStyle,
							v_Dash,
							v_AugmentedRay);
					}
					else if (v_AugmentedPath.size() > 0)
					{
						CDisplayFactory::createAugmentedPath(pcm->displayList,
							featureID,
							v_DrawingPriority,
							v_DisplayPlane,
							v_ViewingGroup,
							v_ScaleMinimum,
							v_LineStyle,
							v_Dash,
							v_ArcByRadius);
					}
					else
					{
						CDisplayFactory::createLineInstruction(pcm->displayList,
							featureID,
							v_DrawingPriority,
							v_DisplayPlane,
							v_ViewingGroup,
							v_ScaleMinimum,
							value,
							v_LineStyle,
							v_Dash,
							vl_SpatialReference);
					}
				}
			}
			break;
			case 16:
			{
				// PointInstruction
				if (tag.compare("PointInstruction") == 0)
				{
					CDisplayFactory::createPointInstruction(pcm->displayList,
						featureID,
						v_DrawingPriority,
						v_DisplayPlane,
						v_ViewingGroup,
						v_ScaleMinimum,
						value,
						v_Rotation,
						v_AugmentedPoint,
						vl_SpatialReference);
				}
				// "SpatialReference:Curve|107" 
				else if (tag.compare("SpatialReference") == 0)
					vl_SpatialReference.push_back(value);
			}
			break;
			case 17:
			{
				// "TextAlignVertical:Center"
				if (tag.compare("TextAlignVertical") == 0)
				{
					v_TextAlignVertical = value;
				}
				else if (tag.compare("AreaFillReference") == 0)
				{
					CDisplayFactory::createAreaInstructionB(pcm->displayList,
						featureID,
						v_DrawingPriority,
						v_DisplayPlane,
						v_ViewingGroup,
						v_ScaleMinimum,
						value);
				}
			}
			break;
			case 19:
			{
				// "TextAlignHorizontal:End"
				if (tag.compare("TextAlignHorizontal") == 0)
				{
					v_TextAlignHorizontal = value;
				}
			}
			break;
			}
		}
	}

	vl_SpatialReference.clear();

	return true;
}

std::wstring ProcessS101::LUA_GetPriority(std::string lua_priority)
{
	return std::wstring(lua_priority.begin(), lua_priority.end());
}

void ProcessS101::InitPortrayal(const char* topLevelRule, S101Cell* cell, FeatureCatalogue* fc, PortrayalCatalogue* pc)
{
	if (theInstance.m_lua_session)
	{
		delete theInstance.m_lua_session;
		theInstance.m_lua_session = nullptr;
	}

	std::string top_level_rule(topLevelRule);
	pTheFC = fc;

	hd_init(cell);

	//Initialize Lua library
	theInstance.m_lua_session = new lua_session();

	//Register host functions.
	theInstance.m_lua_session->register_function("HostGetFeatureIDs", HostGetFeatureIDs);
	theInstance.m_lua_session->register_function("HostFeatureGetCode", HostFeatureGetCode);
	theInstance.m_lua_session->register_function("HostGetInformationTypeIDs", HostGetInformationTypeIDs);
	theInstance.m_lua_session->register_function("HostInformationTypeGetCode", HostInformationTypeGetCode);
	theInstance.m_lua_session->register_function("HostFeatureGetSimpleAttribute", HostFeatureGetSimpleAttribute);
	theInstance.m_lua_session->register_function("HostFeatureGetComplexAttributeCount", HostFeatureGetComplexAttributeCount);
	theInstance.m_lua_session->register_function("HostFeatureGetSpatialAssociations", HostFeatureGetSpatialAssociations);
	theInstance.m_lua_session->register_function("HostFeatureGetAssociatedFeatureIDs", HostFeatureGetAssociatedFeatureIDs);
	theInstance.m_lua_session->register_function("HostFeatureGetAssociatedInformationIDs", HostFeatureGetAssociatedInformationIDs);
	theInstance.m_lua_session->register_function("HostGetSpatialIDs", HostGetSpatialIDs);
	theInstance.m_lua_session->register_function("HostGetSpatial", HostGetSpatial);
	theInstance.m_lua_session->register_function("HostSpatialGetAssociatedInformationIDs", HostSpatialGetAssociatedInformationIDs);
	theInstance.m_lua_session->register_function("HostSpatialGetAssociatedFeatureIDs", HostSpatialGetAssociatedFeatureIDs);
	theInstance.m_lua_session->register_function("HostInformationTypeGetSimpleAttribute", HostInformationTypeGetSimpleAttribute);
	theInstance.m_lua_session->register_function("HostInformationTypeGetComplexAttributeCount", HostInformationTypeGetComplexAttributeCount);
	theInstance.m_lua_session->register_function("HostGetFeatureTypeCodes", HostGetFeatureTypeCodes);
	theInstance.m_lua_session->register_function("HostGetInformationTypeCodes", HostGetInformationTypeCodes);
	theInstance.m_lua_session->register_function("HostGetSimpleAttributeTypeCodes", HostGetSimpleAttributeTypeCodes);
	theInstance.m_lua_session->register_function("HostGetComplexAttributeTypeCodes", HostGetComplexAttributeTypeCodes);
	theInstance.m_lua_session->register_function("HostGetRoleTypeCodes", HostGetRoleTypeCodes);
	theInstance.m_lua_session->register_function("HostGetInformationAssociationTypeCodes", HostGetInformationAssociationTypeCodes);
	theInstance.m_lua_session->register_function("HostGetFeatureAssociationTypeCodes", HostGetFeatureAssociationTypeCodes);
	theInstance.m_lua_session->register_function("HostGetFeatureTypeInfo", HostGetFeatureTypeInfo);
	theInstance.m_lua_session->register_function("HostGetInformationTypeInfo", HostGetInformationTypeInfo);
	theInstance.m_lua_session->register_function("HostGetSimpleAttributeTypeInfo", HostGetSimpleAttributeTypeInfo);
	theInstance.m_lua_session->register_function("HostGetComplexAttributeTypeInfo", HostGetComplexAttributeTypeInfo);
	theInstance.m_lua_session->register_function("HostSpatialRelate", HostSpatialRelate);
	theInstance.m_lua_session->register_function("HostDebuggerEntry", HostDebuggerEntry);
	theInstance.m_lua_session->register_function("HostPortrayalEmit", HostPortrayalEmit);

	// Extract rules path from top level rule file name
	std::string rules_path = top_level_rule.substr(0, top_level_rule.find_last_of('\\'));

	// Setup path so Lua can find the rules files
	std::string path = rules_path + (std::string)"\\?.lua";

	theInstance.m_lua_session->set_field("package", "path", path.c_str());

	// Load top level rule file
	theInstance.m_lua_session->load_file(top_level_rule);

	// Get unknown attribute value string
	g_unknown_attribute_value = theInstance.m_lua_session->call<std::string>("GetUnknownAttributeString", { 1.0 });

	// Turn on type system checks
	theInstance.m_lua_session->call<bool>("TypeSystemChecks", { true });

	// Initialize portrayal context parameters
	theInstance.m_lua_session->get_function("PortrayalInitializeContextParameters");

	auto cps = KRS_LUA_SCRIPT_REFERENCE::GetContextParameters(pc);

	std::vector<lua_variant> cpts;

	for (auto cp : cps)
	{
		cpts.push_back(theInstance.m_lua_session->call("PortrayalCreateContextParameter", { cp.name, cp.type, cp.default_value }));
	}

	theInstance.m_lua_session->push(cpts);

	theInstance.m_lua_session->call_raw(1);
}

void ProcessS101::PortrayalSetContextParameter(const char* parameterName, const char* parameterValue)
{
	theInstance.m_lua_session->call("PortrayalSetContextParameter", { parameterName, parameterValue });
}

