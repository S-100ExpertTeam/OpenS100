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
	DrawingCommand::ViewingGroup viewingGroup;
	DrawingCommand::DisplayPlane displayPlane;
	DrawingCommand::DrawingPriority drawingPriority;
	DrawingCommand::ScaleMinimum scaleMinimum;
	DrawingCommand::ScaleMaximum scaleMaximum;
	DrawingCommand::Id id;
	DrawingCommand::Parent parent;
	DrawingCommand::Hover hover;

	// Transform
	DrawingCommand::LocalOffset localOffset;
	DrawingCommand::LinePlacement linePlacement;
	DrawingCommand::AreaPlacement areaPlacement;
	DrawingCommand::AreaCRS areaCRS;
	DrawingCommand::Rotation rotation;
	DrawingCommand::ScaleFactor scaleFactor;

	// Line Style
	DrawingCommand::LineStyle lineStyle;
	DrawingCommand::LineSymbol lineSymbol;
	DrawingCommand::Dash dash;

	// Text Style
	DrawingCommand::FontColor fontColor;
	DrawingCommand::FontSize fontSize;
	DrawingCommand::FontProportion fontProportion;
	DrawingCommand::FontWeight fontWeight;
	DrawingCommand::FontSlant fontSlant;
	DrawingCommand::FontSerifs fontSerifs;
	DrawingCommand::FontUnderline fontUnderline;
	DrawingCommand::FontStrikethrough fontStrikethrough;
	DrawingCommand::FontUpperline fontUpperline;
	DrawingCommand::FontReference fontReference;
	DrawingCommand::TextAlignHorizontal textAlignHorizontal;
	DrawingCommand::TextAlignVertical textAlignVertical;
	DrawingCommand::TextVerticalOffset textVerticalOffset;

	// Colour Override
	DrawingCommand::OverrideColor overrideColor;
	DrawingCommand::OverrideAll overrideAll;

	// Geometry
	DrawingCommand::SpatialReference spatialReference;
	DrawingCommand::AugmentedPoint augmentedPoint;
	DrawingCommand::AugmentedRay augmentedRay;
	DrawingCommand::AugmentedPath augmentedPath;
	DrawingCommand::Polyline polyline;
	DrawingCommand::Arc3Points arc3Points;
	DrawingCommand::ArcByRadius arcByRadius;
	DrawingCommand::Annulus annulus;
	DrawingCommand::ClearGeometry clearGeometry;

	// Coverage
	DrawingCommand::LookupEntry lookupEntry;
	DrawingCommand::NumericAnnotation numericAnnotation;
	DrawingCommand::SymbolAnnotation symbolAnnotation;
	DrawingCommand::CoverageColor coverageColor;

	// Time
	DrawingCommand::Date date;
	DrawingCommand::Time time;
	DrawingCommand::DateTime dateTime;
	DrawingCommand::TimeValid timeValid;
	DrawingCommand::ClearTime clearTime;

	// Alert
	DrawingCommand::AlertReference alertReference;

	std::list<DrawingCommand::Command*> stateCommands;

public:
	void Init();

	DrawingCommand::Command* Insert(std::string& command, std::string& params);
};

class Local_DrawingCommands
{
public:
	Local_DrawingCommands() = default;
	virtual ~Local_DrawingCommands() = default;

public:
	DrawingCommand::PointInstruction pointInstruction;
	DrawingCommand::LineInstruction lineInstruction;
	DrawingCommand::LineInstructionUnsuppressed lineInstructionUnsuppressed;
	DrawingCommand::ColorFill colorFill;
	DrawingCommand::AreaFillReference areaFillReference;
	DrawingCommand::PixmapFill pixmapFill;	
	DrawingCommand::SymbolFill	symbolFill;
	DrawingCommand::HatchFill hatchFill;
	DrawingCommand::TextInstruction textInstruction;
	DrawingCommand::CoverageFill coverageFill;
	DrawingCommand::NullInstruction nullInstruction;

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