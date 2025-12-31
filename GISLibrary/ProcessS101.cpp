#include "stdafx.h"
#include "ProcessS101.h"
#include "S100Layer.h"
#include "S101Cell.h"
#include "PCOutputSchemaManager.h"
#include "SENC_DisplayList.h"
#include "host_data.h"
#include "host_functions.h"
#include "CommandToInstruction.h"
#include "CommandList.h"

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
#include "../PortrayalCatalogue/ViewingGroupLayer.h"

#include "../LatLonUtility/LatLonUtility.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <sstream> 
#include <algorithm>

using namespace LatLonUtility;

S101LuaScriptingReference ProcessS101::theInstance;
std::string ProcessS101::g_unknown_attribute_value = "";

void Local_StateCommands::Init()
{
	// Visibility
	v_ViewingGroup.clear();
	v_DisplayPlane.clear();
	v_DrawingPriority = "0";
	v_ScaleMinimum = std::to_string(INT32_MAX);
	v_ScaleMaximum = std::to_string(INT32_MIN);
	v_Id.clear();
	v_Parent.clear();
	v_Hover = "false";

	// Transform
	v_LocalOffset;
	v_LinePlacement;
	v_AreaPlacement;
	v_AreaCRS;
	v_Rotation;
	v_ScaleFactor;

	// Line Style
	v_LineStyle;
	v_LineSymbol;
	v_Dash;

	// Text Style
	v_FontColor;
	v_FontSize = "10";
	v_FontProportion;
	v_FontWeight;
	v_FontSlant;
	v_FontSerifs;
	v_FontUnderline;
	v_FontStrikethrough;
	v_FontUpperline;
	v_FontReference;
	v_TextAlignHorizontal;
	v_TextAlignVertical;
	v_TextVerticalOffset;

	// Colour Override
	v_OverrideColor;
	v_OverrideAll;

	// Geometry
	v_SpatialReference;
	v_AugmentedPoint;
	v_AugmentedRay;
	v_AugmentedPath;
	v_Polyline;
	v_Arc3Points;
	v_ArcByRadius;
	v_Annulus;
	v_ClearAugmented;

	// Coverage
	v_LookupEntry;
	v_NumericAnnotation;
	v_SymbolAnnotation;
	v_CoverageColor;

	// Time
	Date;
	Time;
	DateTime;
	TimeValid;
	ClearTime;

	// Alert
	v_AlertReference;

	viewingGroup.init();
	displayPlane.init();
	drawingPriority.init();
	scaleMinimum.init();
	scaleMaximum.init();
	id.init();
	parent.init();
	hover.init();
	
	localOffset.init();
	linePlacement.init();
	areaPlacement.init();
	areaCRS.init();
	rotation.init();
	scaleFactor.init();

	lineStyle.init();
	lineSymbol.init();
	dash.init();

	fontColor.init();
	fontSize.init();
	fontProportion.init();
	fontWeight.init();
	fontSlant.init();
	fontSerifs.init();
	fontUnderline.init();
	fontStrikethrough.init();
	fontUpperline.init();
	fontReference.init();
	textAlignHorizontal.init();
	textAlignVertical.init();
	textVerticalOffset.init();

	overrideColor.init();
	overrideAll.init();

	spatialReference.init();
	augmentedPoint.init();
	augmentedRay.init();
	augmentedPath.init();
	polyline.init();
	arc3Points.init();
	arcByRadius.init();
	annulus.init();
	clearGeometry.init();

	lookupEntry.init();
	numericAnnotation.init();
	symbolAnnotation.init();
	coverageColor.init();

	date.init();
	time.init();
	dateTime.init();
	timeValid.init();
	clearTime.init();

	alertReference.init();
}

void Local_DrawingCommands::Init()
{
	pointInstruction.init();
	lineInstruction.init();
	lineInstructionUnsuppressed.init();
	colorFill.init();
	areaFillReference.init();
	pixmapFill.init();
	symbolFill.init();
	hatchFill.init();
	textInstruction.init();
	coverageFill.init();
	nullInstruction.init();
}

ProcessS101::ProcessS101()
{

}

ProcessS101::~ProcessS101()
{

}

void dump_com_error(_com_error &e)
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

		auto context = pc->GetContext();
		if (!context) {
			return 0;
		}

		//int numContext = context->GetCountOfParameter();
		//for (int i = 0; i < numContext; i++) {
		//	auto contextParameter = context->GetContextParameter(i);
		//	if (contextParameter) {
		//		auto name = contextParameter->GetIdAsString();
		//		auto defaultValue = contextParameter->GetDefaultAsString();
		//		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter(name.c_str(), defaultValue.c_str());
		//	}
		//}

		std::list<Result_DrawingInstruction>* drawingInstructionResult = KRS_LUA_SCRIPT_REFERENCE::GetResultDrawingInstructions();

		int t = (int)drawingInstructionResult->size();

		S101Cell* c = (S101Cell*)layer->m_spatialObject;
		c->InitDrawingInstruction();

		for (auto i = drawingInstructionResult->begin(); i != drawingInstructionResult->end(); i++)
		{
			//std::string drawingInstructions = i->drawingInstructions;
			//std::vector<std::string> di_splited = Split(drawingInstructions, ";");

			LUA_ParsingDrawingInstructions(i->featureID, i->drawingInstructions, c->pcManager);
		}

		c->pcManager->GenerateSENCInstruction(c, layer->GetPC());

		c->pcManager->InitDisplayList();

		KRS_LUA_SCRIPT_REFERENCE::SaveDrawingInstructions("..\\TEMP\\drawingCommands.txt");
		KRS_LUA_SCRIPT_REFERENCE::RemoveResultDrawingInstructions();
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	return 0;
}

int ProcessS101::ProcessS100_XSLT(std::string inputXmlPath, std::string mainRulePath, std::string outputXmlPath, PortrayalCatalogue* pc)
{
	// Initialize the libraries
	xmlInitParser();
	LIBXML_TEST_VERSION
		xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
	xmlSubstituteEntitiesDefault(1);

	// Load XML and XSL
	xmlDocPtr inputXml = xmlParseFile(inputXmlPath.c_str());
	xsltStylesheetPtr xslt = xsltParseStylesheetFile((const xmlChar*)mainRulePath.c_str());

	// Load params
	std::vector<const char*> params;
	std::vector<std::string> temp_params;
	if (pc)
	{
		temp_params = ProcessS101::getParams(pc);
		for (const auto& item : temp_params)
		{
			params.push_back(item.c_str());
		}
	}
	params.push_back(nullptr);

	// Transform
	xmlDocPtr result = xsltApplyStylesheet(xslt, inputXml, params.data());

	// Output the transformed XML
	FILE* outFile = fopen(outputXmlPath.c_str(), "wb");
	xsltSaveResultToFile(outFile, result, xslt);
	fclose(outFile);

	// Cleanup
	xsltFreeStylesheet(xslt);
	xmlFreeDoc(result);
	xmlFreeDoc(inputXml);

	xsltCleanupGlobals();
	xmlCleanupParser();

	return 0;
}

std::string ProcessS101::ProcessS100_XSLT(std::string inputXmlContent, std::string mainRulePath, PortrayalCatalogue* pc)
{
	// Init
	xmlInitParser();
	xsltInit();

	// Load XML & XSLT 
	xmlDocPtr xmlDoc = xmlParseMemory(inputXmlContent.c_str(), inputXmlContent.length());
	if (!xmlDoc) {
		return std::string();
	}

	xsltStylesheetPtr xsltDoc = xsltParseStylesheetFile(reinterpret_cast<const xmlChar*>(mainRulePath.c_str()));
	if (!xsltDoc) {
		xmlFreeDoc(xmlDoc);
		return std::string();
	}

	// Load params
	std::vector<const char*> params;
	std::vector<std::string> temp_params;
	if (pc)
	{
		temp_params = ProcessS101::getParams(pc);
		for (const auto& item : temp_params)
		{
			params.push_back(item.c_str());
		}
	}
	params.push_back(nullptr);

	// Transform XSLT
	xmlDocPtr resultDoc = xsltApplyStylesheet(xsltDoc, xmlDoc, params.data());
	if (!resultDoc) {
		xsltFreeStylesheet(xsltDoc);
		xmlFreeDoc(xmlDoc);
		return std::string();
	}

	// Save result to memory
	xmlChar* resultBuffer = nullptr;
	int bufferSize = 0;
	xsltSaveResultToString(&resultBuffer, &bufferSize, resultDoc, xsltDoc);

	// Convert result to std::string
	std::string result;
	if (resultBuffer) {
		result = std::string(reinterpret_cast<char*>(resultBuffer), bufferSize);
		xmlFree(resultBuffer);
	}
	
	// 메모리 정리
	xmlFreeDoc(resultDoc);
	xsltFreeStylesheet(xsltDoc);
	xmlFreeDoc(xmlDoc);
	xsltCleanupGlobals();
	xmlCleanupParser();

	return result;
}

bool ProcessS101::LUA_ParsingDrawingInstructions(std::string_view featureID, std::string_view drawingCommands, PCOutputSchemaManager* pcm)
{
	std::vector<std::string_view> elements;
	Split(drawingCommands, ";", elements);

	Local_StateCommands stateCommands;
	Local_DrawingCommands drawingCommand;
	CommandList commandList;

	commandList.SetID(std::string(featureID));

	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		std::vector<std::string_view> di_splited;
		std::vector<std::string> cp = Split(std::string(i->data(), i->size()), ":");
		Split(*i, ":", di_splited);

		int splitedSize = (int)di_splited.size();

		if (splitedSize > 0)
		{
			// insert & parse
			commandList.Insert(std::string(featureID), cp[0], cp.size() > 1 ? cp[1] : "");
		}
	}

	auto instructionList = commandList.Parse();
	for (auto instruction : instructionList)
	{
		pcm->displayList->SetDisplayInstruction(instruction);
	}

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

void ProcessS101::PortrayalSetContextParameter(const char*  parameterName, const char*  parameterValue)
{
	theInstance.m_lua_session->call("PortrayalSetContextParameter", { parameterName, parameterValue });
}

std::vector<std::string> ProcessS101::getParams(PortrayalCatalogue* pc)
{
	// Load params
	std::vector<std::string> params;
	if (pc)
	{
		auto context = pc->GetContext();
		if (!context)
		{
			return params;
		}

		int numContext = context->GetCountOfParameter();
		for (int i = 0; i < numContext; i++)
		{
			auto contextParameter = context->GetContextParameter(i);
			if (contextParameter)
			{
				auto name = contextParameter->GetIdAsString();
				auto value = contextParameter->getValueAsString();
				params.push_back(name);
				params.push_back(value);
			}
		}
	}

	return params;
}

bool ProcessS101::IsDrawingCommands(std::string_view str)
{
	if (str.compare("PointInstruction") == 0 ||
		str.compare("LineInstruction") == 0 ||
		str.compare("LineInstructionUnsuppressed") == 0 ||
		str.compare("ColorFill") == 0 ||
		str.compare("AreaFillReference") == 0 ||
		str.compare("PixmapFill") == 0 ||
		str.compare("SymbolFill") == 0 ||
		str.compare("HatchFill") == 0 ||
		str.compare("TextInstruction") == 0 ||
		str.compare("CoverageFill") == 0 ||
		str.compare("NullInstruction") == 0) 
	{
		return true;
	}

	return false;
}
