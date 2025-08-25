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
	DrawingInstructions::ViewingGroup viewingGroup;
	DrawingInstructions::DisplayPlane displayPlane;
	DrawingInstructions::DrawingPriority drawingPriority;
	DrawingInstructions::ScaleMinimum scaleMinimum;
	DrawingInstructions::ScaleMaximum scaleMaximum;
	DrawingInstructions::Id id;
	DrawingInstructions::Parent parent;
	DrawingInstructions::Hover hover;

	// Transform
	DrawingInstructions::LocalOffset localOffset;
	DrawingInstructions::LinePlacement linePlacement;
	DrawingInstructions::AreaPlacement areaPlacement;
	DrawingInstructions::AreaCRS areaCRS;
	DrawingInstructions::Rotation rotation;
	DrawingInstructions::ScaleFactor scaleFactor;

	// Line Style
	DrawingInstructions::LineStyle lineStyle;
	DrawingInstructions::LineSymbol lineSymbol;
	DrawingInstructions::Dash dash;

	// Text Style
	DrawingInstructions::FontColor fontColor;
	DrawingInstructions::FontSize fontSize;
	DrawingInstructions::FontProportion fontProportion;
	DrawingInstructions::FontWeight fontWeight;
	DrawingInstructions::FontSlant fontSlant;
	DrawingInstructions::FontSerifs fontSerifs;
	DrawingInstructions::FontUnderline fontUnderline;
	DrawingInstructions::FontStrikethrough fontStrikethrough;
	DrawingInstructions::FontUpperline fontUpperline;
	DrawingInstructions::FontReference fontReference;
	DrawingInstructions::TextAlignHorizontal textAlignHorizontal;
	DrawingInstructions::TextAlignVertical textAlignVertical;
	DrawingInstructions::TextVerticalOffset textVerticalOffset;

	// Colour Override
	DrawingInstructions::OverrideColor overrideColor;
	DrawingInstructions::OverrideAll overrideAll;

	// Geometry
	DrawingInstructions::SpatialReference spatialReference;
	DrawingInstructions::AugmentedPoint augmentedPoint;
	DrawingInstructions::AugmentedRay augmentedRay;
	DrawingInstructions::AugmentedPath augmentedPath;
	DrawingInstructions::Polyline polyline;
	DrawingInstructions::Arc3Points arc3Points;
	DrawingInstructions::ArcByRadius arcByRadius;
	DrawingInstructions::Annulus annulus;
	DrawingInstructions::ClearGeometry clearGeometry;

	// Coverage
	DrawingInstructions::LookupEntry lookupEntry;
	DrawingInstructions::NumericAnnotation numericAnnotation;
	DrawingInstructions::SymbolAnnotation symbolAnnotation;
	DrawingInstructions::CoverageColor coverageColor;

	// Time
	DrawingInstructions::Date date;
	DrawingInstructions::Time time;
	DrawingInstructions::DateTime dateTime;
	DrawingInstructions::TimeValid timeValid;
	DrawingInstructions::ClearTime clearTime;

	// Alert
	DrawingInstructions::AlertReference alertReference;

	std::list<DrawingInstructions::Command*> stateCommands;

public:
	void Init();

	DrawingInstructions::Command* Insert(std::string& command, std::string& params);
};

class Local_DrawingCommands
{
public:
	Local_DrawingCommands() = default;
	virtual ~Local_DrawingCommands() = default;

public:
	DrawingInstructions::PointInstruction pointInstruction;
	DrawingInstructions::LineInstruction lineInstruction;
	DrawingInstructions::LineInstructionUnsuppressed lineInstructionUnsuppressed;
	DrawingInstructions::ColorFill colorFill;
	DrawingInstructions::AreaFillReference areaFillReference;
	DrawingInstructions::PixmapFill pixmapFill;	
	DrawingInstructions::SymbolFill	symbolFill;
	DrawingInstructions::HatchFill hatchFill;
	DrawingInstructions::TextInstruction textInstruction;
	DrawingInstructions::CoverageFill coverageFill;
	DrawingInstructions::NullInstruction nullInstruction;

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