#pragma once

#include "S101LuaScriptingReference.h"
#include "VisibilityCommands.h"
#include "TransformCommands.h"
#include "LineStyleCommands.h"
#include "TextStyleCommands.h"
#include "ColourOverrideCommands.h"
#include "GeometryCommands.h"
#include "CoverageCommands.h"
#include"TimeCommands.h"
#include "AlertCommands.h"
#include "DrawingCommands.h"
#include "Command.h"
#include "PointInstruction.h"

#include <string>
#include <vector>
#include <list>

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

	// Visibility
	Part9a::ViewingGroup viewingGroup;
	Part9a::DisplayPlane displayPlane;
	Part9a::DrawingPriority drawingPriority;
	Part9a::ScaleMinimum scaleMinimum;
	Part9a::ScaleMaximum scaleMaximum;
	Part9a::Id id;
	Part9a::Parent parent;
	Part9a::Hover hover;

	// Transform
	Part9a::LocalOffset localOffset;
	Part9a::LinePlacement linePlacement;
	Part9a::AreaPlacement areaPlacement;
	Part9a::AreaCRS areaCRS;
	Part9a::Rotation rotation;
	Part9a::ScaleFactor scaleFactor;

	// Line Style
	Part9a::LineStyle lineStyle;
	Part9a::LineSymbol lineSymbol;
	Part9a::Dash dash;

	// Text Style
	Part9a::FontColor fontColor;
	Part9a::FontSize fontSize;
	Part9a::FontProportion fontProportion;
	Part9a::FontWeight fontWeight;
	Part9a::FontSlant fontSlant;
	Part9a::FontSerifs fontSerifs;
	Part9a::FontUnderline fontUnderline;
	Part9a::FontStrikethrough fontStrikethrough;
	Part9a::FontUpperline fontUpperline;
	Part9a::FontReference fontReference;
	Part9a::TextAlignHorizontal textAlignHorizontal;
	Part9a::TextAlignVertical textAlignVertical;
	Part9a::TextVerticalOffset textVerticalOffset;

	// Colour Override
	Part9a::OverrideColor overrideColor;
	Part9a::OverrideAll overrideAll;

	// Geometry
	Part9a::SpatialReference spatialReference;
	Part9a::AugmentedPoint augmentedPoint;
	Part9a::AugmentedRay augmentedRay;
	Part9a::AugmentedPath augmentedPath;
	Part9a::Polyline polyline;
	Part9a::Arc3Points arc3Points;
	Part9a::ArcByRadius arcByRadius;
	Part9a::Annulus annulus;
	Part9a::ClearGeometry clearGeometry;

	// Coverage
	Part9a::LookupEntry lookupEntry;
	Part9a::NumericAnnotation numericAnnotation;
	Part9a::SymbolAnnotation symbolAnnotation;
	Part9a::CoverageColor coverageColor;

	// Time
	Part9a::Date date;
	Part9a::Time time;
	Part9a::DateTime dateTime;
	Part9a::TimeValid timeValid;
	Part9a::ClearTime clearTime;

	// Alert
	Part9a::AlertReference alertReference;

	std::list<Part9a::Command*> stateCommands;

public:
	void Init();

	Part9a::Command* Insert(std::string& command, std::string& params);
};

class Local_DrawingCommands
{
public:
	Local_DrawingCommands() = default;
	virtual ~Local_DrawingCommands() = default;

public:
	Part9a::PointInstruction pointInstruction;
	Part9a::LineInstruction lineInstruction;
	Part9a::LineInstructionUnsuppressed lineInstructionUnsuppressed;
	Part9a::ColorFill colorFill;
	Part9a::AreaFillReference areaFillReference;
	Part9a::PixmapFill pixmapFill;	
	Part9a::SymbolFill	symbolFill;
	Part9a::HatchFill hatchFill;
	Part9a::TextInstruction textInstruction;
	Part9a::CoverageFill coverageFill;
	Part9a::NullInstruction nullInstruction;

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