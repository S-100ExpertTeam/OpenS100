#pragma once

#include "S101LuaScriptingReference.h"
#include "VisibilityCommands.h"

#include <string>
#include <vector>

class S100Layer;
class PCOutputSchemaManager;

class Local_StateCommands
{
public:
	Local_StateCommands() = default;
	virtual ~Local_StateCommands() = default;
	
public:
	// Visibility
	std::vector<std::string_view> v_ViewingGroup;
	DrawingInstructions::ViewingGroup viewingGroup;

	std::string v_DisplayPlane;
	std::string v_DrawingPriority;
	std::string v_ScaleMinimum;
	std::string v_ScaleMaximum;
	std::string v_Id;
	std::string v_Parent;
	std::string v_Hover;

	// Transform
	std::string v_LocalOffset;
	std::string v_LinePlacement;
	std::string v_AreaPlacement;
	std::string v_AreaCRS;
	std::string v_Rotation;
	std::string v_ScaleFactor;

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

	// Time
	std::string Date;
	std::string Time;
	std::string DateTime;
	std::string TimeValid;
	std::string ClearTime;

	// Alert
	std::string v_AlertReference;

public:
	void Init();
};

class ProcessS101
{
public:
	ProcessS101();
	virtual ~ProcessS101();

public:
	static S101LuaScriptingReference theInstance;
	static std::string g_unknown_attribute_value;

public:
	static int ProcessS101_LUA(std::wstring luaRulePath, S100Layer* layer);
	static int ProcessS100_XSLT(std::string inputXmlPath, std::string mainRulePath, std::string outputXmlPath, PortrayalCatalogue* pc = nullptr);
	static std::string ProcessS100_XSLT(std::string inputXmlContent, std::string mainRulePath, PortrayalCatalogue* pc = nullptr);

	//static bool LUA_ParsingDrawingInstructions(std::string featureID, std::vector<std::string> elements, PCOutputSchemaManager* pcm);
	static bool LUA_ParsingDrawingInstructions(std::string_view featureID, std::string_view drawingCommands, PCOutputSchemaManager* pcm);
	static std::wstring LUA_GetPriority(std::string lua_priority);

	static void InitPortrayal( 
		const char* topLevelRule, 
		S101Cell* cell, 
		FeatureCatalogue* fc, 
		PortrayalCatalogue* pc);

	static void InitPortrayal(
		const char* topLevelRule,
		S100SpatialObject* cell,
		FeatureCatalogue* fc,
		PortrayalCatalogue* pc);

	static void PortrayalSetContextParameter(
		const char* parameterName, 
		const char* parameterValue);

	// Load params
	static std::vector<std::string> getParams(PortrayalCatalogue * pc);

private:
	static bool IsDrawingCommands(std::string_view str);
};