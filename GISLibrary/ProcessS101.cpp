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
	v_DisplayPlane = "";
	v_DrawingPriority = "0";
	v_ScaleMinimum = std::to_string(INT32_MAX);
	v_ScaleMaximum = std::to_string(INT32_MIN);
	v_Id = "";
	v_Parent = "";
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

		int numContext = context->GetCountOfParameter();
		for (int i = 0; i < numContext; i++) {
			auto contextParameter = context->GetContextParameter(i);
			if (contextParameter) {
				auto name = contextParameter->GetIdAsString();
				auto defaultValue = contextParameter->GetDefaultAsString();
				KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter(name.c_str(), defaultValue.c_str());
			}
		}

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
	if (featureID == "195")
	{
		OutputDebugString(L"Feature ID 195 encountered.");
	}

	std::vector<std::string_view> elements;
	Split(drawingCommands, ";", elements);

	Local_StateCommands stateCommands;

	std::string v_ColorFill;
	std::string v_TextInstruction;
	std::string v_LineInstruction;
	std::string v_PointInstruction;
	std::list<std::string_view> vl_SpatialReference;
	std::string v_AreaFillReference;
	S100_Dash dash;

	S100_LineStyle lineStyle;

	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		std::vector<std::string_view> di_splited;
		std::vector<std::string> cp = Split(i->data(), ":");;
		Split(*i, ":", di_splited);

		int splitedSize = (int)di_splited.size();
		if (splitedSize > 0)
		{
			if (di_splited[0].compare("PointInstruction") == 0)
			{

			}
			else if (di_splited[0].compare("LineInstruction") == 0)
			{
				if (splitedSize > 1)
				{
					v_LineInstruction = di_splited[1];
					stateCommands.v_LineStyle = v_LineInstruction;
					lineStyle.ParseValue(v_LineInstruction);
				}
			}
			else if (di_splited[0].compare("LineInstructionUnsuppressed") == 0)
			{
				if (splitedSize > 1)
				{
					v_LineInstruction = di_splited[1];
					stateCommands.v_LineStyle = v_LineInstruction;
					lineStyle.ParseValue(v_LineInstruction);
				}
			}
			else if (di_splited[0].compare("ColorFill") == 0)
			{
				if (splitedSize > 1)
				{
					v_ColorFill = di_splited[1];
				}
			}
			else if (di_splited[0].compare("AreaFillReference") == 0)
			{
				if (splitedSize > 1)
				{
					v_AreaFillReference = di_splited[1];
				}
			}
			else if (di_splited[0].compare("PixmapFill") == 0)
			{
			}
			else if (di_splited[0].compare("SymbolFill") == 0)
			{
			}
			else if (di_splited[0].compare("HatchFill") == 0)
			{
			}
			else if (di_splited[0].compare("TextInstruction") == 0)
			{
				if (splitedSize > 1)
				{
					v_TextInstruction = di_splited[1];
					stateCommands.v_TextAlignHorizontal = v_TextInstruction;
				}
			}
			else if (di_splited[0].compare("CoverageFill") == 0)
			{
			}
			else if (di_splited[0].compare("NullInstruction") == 0)
			{
			}
			else if (di_splited[0].compare("ViewingGroup") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.viewingGroup.parse(cp[1]);
				}
			}
			else if (di_splited[0].compare("DisplayPlane") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_DisplayPlane = di_splited[1];
					//stateCommands.displayPlane.ParseValue(stateCommands.v_DisplayPlane);
				}
			}
			else if (di_splited[0].compare("DrawingPlane") == 0)
			{
			}
			else if (di_splited[0].compare("DrawingGroup") == 0)
			{
			}
			else if (di_splited[0].compare("DrawingPriority") == 0)
			{

			}
			else if (di_splited[0].compare("ScaleMinimum") == 0)
			{

			}
			else if (di_splited[0].compare("ScaleMaximum") == 0)
			{

			}
			else if (di_splited[0].compare("Id") == 0)
			{

			}
			else if (di_splited[0].compare("Parent") == 0)
			{

			}
			else if (di_splited[0].compare("Hover") == 0)
			{

			}
			else if (di_splited[0].compare("LocalOffset") == 0)
			{

			}
			else if (di_splited[0].compare("LinePlacement") == 0)
			{

			}
			else if (di_splited[0].compare("AreaPlacement") == 0)
			{
			}
			else if (di_splited[0].compare("AreaCRS") == 0)
			{
			}
			else if (di_splited[0].compare("Rotation") == 0)
			{
			}
			else if (di_splited[0].compare("ScaleFactor") == 0)
			{
			}
			else if (di_splited[0].compare("LineStyle") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_LineStyle = di_splited[1];
					lineStyle.ParseValue(stateCommands.v_LineStyle);
					if (false == dash.IsEmpty())
					{
						lineStyle.SetDash(&dash);
						dash.SetEmpty();
					}
				}
			}
			else if (di_splited[0].compare("LineSymbol") == 0)
			{
			}
			else if (di_splited[0].compare("Dash") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_Dash = di_splited[1];
					dash.ParseValue(stateCommands.v_Dash);
				}
			}
			else if (di_splited[0].compare("FontColor") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_FontColor = di_splited[1];
				}
			}
			else if (di_splited[0].compare("FontSize") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_FontSize = di_splited[1];
				}
			}
			else if (di_splited[0].compare("FontProportion") == 0)
			{
			}
			else if (di_splited[0].compare("FontWeight") == 0)
			{
			}
			else if (di_splited[0].compare("FontSlant") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_FontSlant = di_splited[1];
				}
			}
			else if (di_splited[0].compare("FontSerifs") == 0)
			{
			}
			else if (di_splited[0].compare("FontUnderline") == 0)
			{
			}
			else if (di_splited[0].compare("FontStrikethrough") == 0)
			{
			}
			else if (di_splited[0].compare("FontUpperline") == 0)
			{
			}
			else if (di_splited[0].compare("FontReference") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_FontReference = di_splited[1];
				}
			}
			else if (di_splited[0].compare("TextAlignHorizontal") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_TextAlignHorizontal = di_splited[1];
				}
			}
			else if (di_splited[0].compare("TextAlignVertical") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_TextAlignVertical = di_splited[1];
				}
			}
			else if (di_splited[0].compare("TextVerticalOffset") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_TextVerticalOffset = di_splited[1];
				}
			}
			else if (di_splited[0].compare("OverrideColor") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_OverrideColor = di_splited[1];
				}
			}
			else if (di_splited[0].compare("OverrideAll") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_OverrideAll = di_splited[1];
				}
			}
			else if (di_splited[0].compare("SpatialReference") == 0)
			{
				if (splitedSize > 1)
				{
					std::string_view spatialRef = di_splited[1];
					if (!spatialRef.empty())
					{
						//stateCommands.v_SpatialReference.push_back(spatialRef);
					}
				}
			}
			else if (di_splited[0].compare("AugmentedPoint") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_AugmentedPoint;
				}
			}
			else if (di_splited[0].compare("AugmentedRay") == 0)
			{

			}
			else if (di_splited[0].compare("AugmentedPath") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_AugmentedPath = di_splited[1];
				}
			}
			else if (di_splited[0].compare("Polyline") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_Polyline = di_splited[1];
				}
			}
			else if (di_splited[0].compare("Arc3Points") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_Arc3Points = di_splited[1];
				}
			}
			else if (di_splited[0].compare("ArcByRadius") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_ArcByRadius = di_splited[1];
				}
			}
			else if (di_splited[0].compare("Annulus") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_Annulus = di_splited[1];
				}
			}
			else if (di_splited[0].compare("ClearAugmented") == 0)
			{
				stateCommands.v_ClearAugmented = di_splited[1];
			}
			else if (di_splited[0].compare("LookupEntry") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_LookupEntry = di_splited[1];
				}
			}
			else if (di_splited[0].compare("NumericAnnotation") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_NumericAnnotation = di_splited[1];
				}
			}
			else if (di_splited[0].compare("SymbolAnnotation") == 0)
			{
			}
			else if (di_splited[0].compare("CoverageColor") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_CoverageColor = di_splited[1];
				}
			}
			else if (di_splited[0].compare("Date") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.Date = di_splited[1];
				}
			}
			else if (di_splited[0].compare("Time") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.Time = di_splited[1];
				}
			}
			else if (di_splited[0].compare("DateTime") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.DateTime = di_splited[1];
				}
			}
			else if (di_splited[0].compare("TimeValid") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.TimeValid = di_splited[1];
				}
			}
			else if (di_splited[0].compare("ClearTime") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.ClearTime = di_splited[1];
				}
			}
			else if (di_splited[0].compare("AlertReference") == 0)
			{
				if (splitedSize > 1)
				{
					stateCommands.v_AlertReference = di_splited[1];
				}
			}
		}

		if (splitedSize == 1)
		{
			// "NullInstruction"
			// "ClearGeometry"
			if (di_splited[0].compare("ClearGeometry") == 0)
			{
				stateCommands.v_AugmentedPoint = "";
				vl_SpatialReference.clear();
				v_AreaFillReference = "";
				stateCommands.v_AugmentedRay = "";
				stateCommands.v_AugmentedPath = "";
				stateCommands.v_ArcByRadius = "";
			}
		}
		else
		{
			// Splited Size 2
			std::string_view tag = di_splited[0];
			std::string_view value = di_splited[1];

			int sizeForIndex = (int)tag.size();

			if (tag.compare("TextVerticalOffset") == 0)
			{
				OutputDebugString(L"A");
			}
			else if (sizeForIndex == 2)
			{
			}
			else if (sizeForIndex == 4)
			{
				// "Dash:0,3.6"
				if (tag.compare("Dash") == 0)
				{
					stateCommands.v_Dash = value;
					dash.ParseValue(value);
				}
			}
			else if (sizeForIndex == 5)
			{
				if (tag.compare("Hover") == 0)
				{
				}
			}
			else if (sizeForIndex == 6)
			{
				if (tag.compare("Parent") == 0)
				{
				}
			}
			else if (sizeForIndex == 8)
			{
				// "FontSize:10"
				if (tag.compare("FontSize") == 0)
				{
					stateCommands.v_FontSize = value;
				}
				// "Rotation:PortrayalCRS,135"
				else if (tag.compare("Rotation") == 0)
				{
					stateCommands.v_Rotation = value;
				}
			}
			else if (sizeForIndex == 7)
			{
				if (tag.compare("AreaCRS") == 0)
				{
				}
			}
			else if (sizeForIndex == 9)
			{
				// "ColorFill:DEPVS"
				if (tag.compare("ColorFill") == 0)
				{
					v_ColorFill = value;

					S100_AreaInstruction *in = new S100_AreaInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
					in->SetViewingGroup(stateCommands.v_ViewingGroup);
					in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

					if (v_ColorFill.size() > 0)
					{
						std::vector<std::string> v_splited = Split(v_ColorFill, ",");

						if (!in->GetAreaFill()) in->SetAreaFill(new S100_ColorFill());
						S100_ColorFill* cf = (S100_ColorFill*)in->GetAreaFill();

						if (!cf->GetColor()) cf->SetColor(new S100_Color());
						cf->GetColor()->SetToken(std::wstring(v_splited[0].begin(), v_splited[0].end()));
						cf->GetColor()->SetName(std::wstring(v_splited[0].begin(), v_splited[0].end()));

						if (v_splited.size() > 1)
						{
							cf->GetColor()->SetTransparency(std::wstring(v_splited[1].begin(), v_splited[1].end()));
						}
					}
					v_ColorFill = "";
				}
				else if (tag.compare("LineStyle") == 0)
				{
					stateCommands.v_LineStyle = value;
					lineStyle.ParseValue(value);

					if (false == dash.IsEmpty())
					{
						lineStyle.SetDash(&dash);
						dash.SetEmpty();
					}
				}
				else if (tag.compare("FontColor") == 0)
				{
					stateCommands.v_FontColor = value;
				}
				else if (tag.compare("FontSlant") == 0)
				{
					stateCommands.v_FontSlant = value;
				}
			}
			else if (sizeForIndex == 11)
			{
				// "LocalOffset:-3.51,3.51"
				if (tag.compare("LocalOffset") == 0)
				{
					stateCommands.v_LocalOffset = value;
				}
				// "ScaleFactor:0.311"
				else if (tag.compare("ScaleFactor") == 0)
				{
					stateCommands.v_ScaleFactor = value;
				}
				// "ArcByRadius:0,0,20,24,199"
				else if (tag.compare("ArcByRadius") == 0)
				{
					stateCommands.v_ArcByRadius = value;
				}
			}
			else if (sizeForIndex == 12)
			{
				// "ScaleMinimum:179999"
				if (tag.compare("ScaleMinimum") == 0)
				{
					stateCommands.v_ScaleMinimum = value;
				}
				// "ViewingGroup:27020"
				else if (tag.compare("ViewingGroup") == 0)
				{
					stateCommands.v_ViewingGroup.push_back(value);
				}
				// "DisplayPlane:OverRADAR"
				else if (tag.compare("DisplayPlane") == 0)
				{
					stateCommands.v_DisplayPlane = value;
				}
				// "AugmentedRay:GeographicCRS,24,GeographicCRS,16668"
				else if (tag.compare("AugmentedRay") == 0)
				{
					stateCommands.v_AugmentedRay = value;

					stateCommands.v_AugmentedPath = "";
				}
			}
			else if (sizeForIndex == 13)
			{
				if (tag.compare("LinePlacement") == 0)
				{
					stateCommands.v_LinePlacement = value;
				}
				else if (tag.compare("AugmentedPath") == 0)
				{
					stateCommands.v_AugmentedPath = value;
					stateCommands.v_AugmentedRay = "";
				}
				else if (tag.compare("AreaPlacement") == 0)
				{
				}
			}
			else if (sizeForIndex == 14)
			{
				if (tag.compare("AugmentedPoint") == 0)
				{
					stateCommands.v_AugmentedPoint = value;
				}
				else if (tag.compare("AlertReference") == 0)
				{
					// !Don't parsing AlertReference util new model of drawingCommands is completed... 
					
					//v_AlertReference = value;

					//S100_AlertReference* in = new S100_AlertReference();
					//pcm->displayList->AddAlertInstruction((S100_Instruction*)in);

					//in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));

					//std::vector<std::string> v_splited = Split(v_AlertReference, ",");
					//if (v_splited.size() > 0)
					//{
					//	in->alertType = std::wstring(v_splited[0].begin(), v_splited[0].end());
					//}
					//if (v_splited.size() > 1)
					//{
					//	in->plan = std::wstring(v_splited[1].begin(), v_splited[1].end());
					//}
					//if (v_splited.size() > 2)
					//{
					//	in->monitor = std::wstring(v_splited[2].begin(), v_splited[2].end());
					//}

					//v_AlertReference = "";
				}
			}
			else if (sizeForIndex == 15)
			{
				// "DrawingPriority:8"
				if (tag.compare("DrawingPriority") == 0)
				{
					stateCommands.v_DrawingPriority = value;
				}
				// "TextInstruction:bn Alligator River Light 16,21,8"
				else if (tag.compare("TextInstruction") == 0)
				{
					v_TextInstruction = value;
					S100_TextInstruction *in = new S100_TextInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
					in->SetViewingGroup(stateCommands.v_ViewingGroup);
					in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

					in->SetTextPoint(new S100_TextPoint());

					if (stateCommands.v_TextAlignVertical.size() > 0)
					{
						in->GetTextPoint()->SetVerticalAlignment(std::wstring(stateCommands.v_TextAlignVertical.begin(), stateCommands.v_TextAlignVertical.end()));
					}
					if (stateCommands.v_TextAlignHorizontal.size() > 0)
					{
						in->GetTextPoint()->SetHorizontalAlignment(std::wstring(stateCommands.v_TextAlignHorizontal.begin(), stateCommands.v_TextAlignHorizontal.end()));
					}

					if (stateCommands.v_LocalOffset.size() > 0)
					{
						in->GetTextPoint()->SetOffset(new S100_VectorPoint());

						std::vector<std::string> v_splited = Split(stateCommands.v_LocalOffset, ",");
						if (v_splited.size() == 2)
						{
							in->GetTextPoint()->GetOffset()->SetX(std::wstring(v_splited[0].begin(), v_splited[0].end()));
							in->GetTextPoint()->GetOffset()->SetY(std::wstring(v_splited[1].begin(), v_splited[1].end()));
						}
						else
						{
							//OutputDebugString(L"Error : Offset Value should have 2 arguments.");
						}
					}

					if (v_TextInstruction.size() > 0)
					{
						LatLonUtility::replace_string(v_TextInstruction, "&s", ";");
						LatLonUtility::replace_string(v_TextInstruction, "&c", ":");
						LatLonUtility::replace_string(v_TextInstruction, "&m", ",");
						LatLonUtility::replace_string(v_TextInstruction, "&a", "&");

						S100_Element* element = new S100_Element();

						in->GetTextPoint()->SetElement(element);

						if (!element->GetText())
						{
							element->SetText(new S100_Text());
						}

						//std::vector<std::string> v_splited_text = Split(v_TextInstruction, ",");

						//auto wValue = LibMFCUtil::ConvertCtoWC((char*)v_splited_text[0].c_str());
						auto wValue = LibMFCUtil::ConvertCtoWC((char*)v_TextInstruction.c_str());
						std::wstring wstrValue = wValue;
						delete[] wValue;
						element->GetText()->SetValue(wstrValue);

						if (stateCommands.v_FontSize.size() > 0)
						{
							element->SetBodySize(std::wstring(stateCommands.v_FontSize.begin(), stateCommands.v_FontSize.end()));
						}

						if (stateCommands.v_FontSlant.size() > 0)
						{
							if (!element->GetFont())
							{
								element->SetFont(new S100_Font());
							}

							element->GetFont()->SetSlant(std::wstring(stateCommands.v_FontSlant.begin(), stateCommands.v_FontSlant.end()));
						}

						if (stateCommands.v_FontColor.size() > 0)
						{
							auto fontColor = new S100_Foreground();
							fontColor->fromDrawingCommand(stateCommands.v_FontColor);
							element->SetForground(fontColor);
						}
					}

					v_TextInstruction = "";
				}
				else if (tag.compare("LineInstruction") == 0)
				{
					v_LineInstruction = value;

					if (stateCommands.v_AugmentedRay.size() > 0)
					{
						S100_AugmentedRay *in = new S100_AugmentedRay();
						pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

						in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
						in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
						in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
						in->SetViewingGroup(stateCommands.v_ViewingGroup);
						in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

						if (false == lineStyle.IsEmpty())
						{
							in->SetLineStyle(new S100_LineStyle(lineStyle));
						}

						if (stateCommands.v_AugmentedRay.size() > 0)
						{
							std::vector<std::string> v_splited = Split(stateCommands.v_AugmentedRay, ",");
							if (v_splited.size() == 4)
							{
								in->SetDirection(std::wstring(v_splited[1].begin(), v_splited[1].end()));
								in->SetLength(std::wstring(v_splited[3].begin(), v_splited[3].end()));
							}
						}
					}
					else if (stateCommands.v_AugmentedPath.size() > 0)
					{
						S100_AugmentedPath *in = new S100_AugmentedPath();
						pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

						in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
						in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
						in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
						in->SetViewingGroup(stateCommands.v_ViewingGroup);
						in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

						if (false == lineStyle.IsEmpty())
						{
							in->SetLineStyle(new S100_LineStyle(lineStyle));
						}

						if (stateCommands.v_ArcByRadius.size() > 0)
						{
							std::vector<std::string> v_splited = Split(stateCommands.v_ArcByRadius, ",");
							if (v_splited.size() == 5)
							{

								if (!in->GetPath()) in->SetPath(new S100_Path());

								S100_ArcByRadius arcByRadius;

								arcByRadius.GetCenter()->SetX(std::wstring(v_splited[0].begin(), v_splited[0].end()));
								arcByRadius.GetCenter()->SetY(std::wstring(v_splited[1].begin(), v_splited[1].end()));
								arcByRadius.SetRadius(std::wstring(v_splited[2].begin(), v_splited[2].end()));

								if (arcByRadius.GetSector() == nullptr)arcByRadius.SetSector(new S100_Sector());
								arcByRadius.GetSector()->SetStartAngle(std::wstring(v_splited[3].begin(), v_splited[3].end()));
								arcByRadius.GetSector()->SetAnglearDistance(std::wstring(v_splited[4].begin(), v_splited[4].end()));


								in->GetPath()->SetArcByRadiuses(&arcByRadius);
							}
						}
					}
					else
					{
						S100_LineInstruction *in = new S100_LineInstruction();
						pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

						in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
						in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
						in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
						in->SetViewingGroup(stateCommands.v_ViewingGroup);
						in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

						if (v_LineInstruction.size() > 0)
						{
							if (!in->GetLineStyleReference())
							{
								in->SetLineStyleReference(new S100_LineStyleReference());
								in->GetLineStyleReference()->SetReference(std::wstring(v_LineInstruction.begin(), v_LineInstruction.end()));
							}
						}

						if (false == lineStyle.IsEmpty())
						{
							in->SetLineStyle(new S100_LineStyle(lineStyle));
						}

						if (vl_SpatialReference.size() > 0)
						{
							for (auto it = vl_SpatialReference.begin(); it != vl_SpatialReference.end(); it++)
							{
								S100_SpatialReference* sref = new S100_SpatialReference();
								in->SetSpatialReference(sref);
								std::string_view v_SpatialReference = *it;

								std::vector<std::string_view> v_splited;
								Split(v_SpatialReference, "|", v_splited);
								if (v_splited.size() == 2)
								{
									sref->SetType(v_splited[0]);
									sref->SetReference(v_splited[1]);
								}
							}
							v_LineInstruction = "";
						}
					}
				}
			}
			else if (sizeForIndex == 16)
			{
				// PointInstruction
				if (tag.compare("PointInstruction") == 0)
				{
					v_PointInstruction = value;
					S100_PointInstruction *in = new S100_PointInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
					in->SetViewingGroup(stateCommands.v_ViewingGroup);
					in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

					if (v_PointInstruction.size() > 0)
					{
						in->SetSymbol(new S100_Symbol());
						in->GetSymbol()->SetReference(std::wstring(v_PointInstruction.begin(), v_PointInstruction.end()));

						std::vector<std::string> r_splited = Split(stateCommands.v_Rotation, ",");
						if (r_splited.size() == 2)
						{
							in->GetSymbol()->SetRotation(std::stod(r_splited[1]));

						}
					}

					if (stateCommands.v_AugmentedPoint.size() > 1)
					{
						std::vector<std::string> v_splited = Split(stateCommands.v_AugmentedPoint, ",");
						if (v_splited.size() == 3)
						{
							if (!in->GetVectorPoint()) in->SetVectorPoint(new S100_VectorPoint());

							in->GetVectorPoint()->SetX(std::wstring(v_splited[1].begin(), v_splited[1].end()));
							in->GetVectorPoint()->SetY(std::wstring(v_splited[2].begin(), v_splited[2].end()));
						}
						else
						{
							//OutputDebugString(L"Error : Vector Point Value should have 3 arguments.");
						}
					}

					if (vl_SpatialReference.size() > 0)
					{
						for (auto it = vl_SpatialReference.begin(); it != vl_SpatialReference.end(); it++)
						{
							S100_SpatialReference* sref = new S100_SpatialReference();
							in->SetSpatialReference(sref);
							std::string_view v_SpatialReference = *it;

							std::vector<std::string_view> v_splited;
							Split(v_SpatialReference, "|", v_splited);
							if (v_splited.size() == 2)
							{
								sref->SetType(v_splited[0]);
								sref->SetReference(v_splited[1]);
							}
						}
					}

					v_PointInstruction = "";
				}
				// "SpatialReference:Curve|107" 
				else if (tag.compare("SpatialReference") == 0)
				{
					std::string_view v_SpatialReference = value;
					vl_SpatialReference.push_back(v_SpatialReference);
				}
			}
			else if (sizeForIndex == 17)
			{
				// "TextAlignVertical:Center"
				if (tag.compare("TextAlignVertical") == 0)
				{
					stateCommands.v_TextAlignVertical = value;
				}
				else if (tag.compare("AreaFillReference") == 0)
				{
					v_AreaFillReference = value;

					S100_AreaInstruction *in = new S100_AreaInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->SetFeatureReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingPriority(LUA_GetPriority(stateCommands.v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(stateCommands.v_DisplayPlane.begin(), stateCommands.v_DisplayPlane.end()));
					in->SetViewingGroup(stateCommands.v_ViewingGroup);
					in->SetScaleMinimum(std::wstring(stateCommands.v_ScaleMinimum.begin(), stateCommands.v_ScaleMinimum.end()));

					if (v_AreaFillReference.size() > 0)
					{
						if (!in->GetAreaFill())
						{
							in->SetAreaFill(new S100_AreaFillReference());
						}

						S100_AreaFillReference* cf = (S100_AreaFillReference*)in->GetAreaFill();

						cf->SetReference(std::wstring(v_AreaFillReference.begin(), v_AreaFillReference.end()));

						std::wstring path = cf->GetReference();
						path.append(L".xml");
						in->SetAreaFill(cf);
					}
					v_AreaFillReference = "";
				}
			}
			else if (sizeForIndex == 19)
			{
				// "TextAlignHorizontal:End"
				if (tag.compare("TextAlignHorizontal") == 0)
				{
					stateCommands.v_TextAlignHorizontal = value;
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag, sizeForIndex);
					//OutputDebugString(str);
				}
			}
		}
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