#include "stdafx.h"
#include "CommandList.h"
#include "VisibilityCommands.h"
#include "TransformCommands.h"
#include "LineStyleCommands.h"
#include "TextStyleCommands.h"
#include "ColourOverrideCommands.h"
#include "GeometryCommands.h"
#include "CoverageCommands.h"
#include "TimeCommands.h"
#include "AlertCommands.h"
#include "DrawingCommands.h"

#include "..\\PortrayalCatalogue\\S100_Instruction.h"

#include <unordered_map>
#include <memory>

void CommandList::Insert(std::string& command, std::string& params)
{
	using namespace DrawingInstructions;

	static const std::unordered_map<std::string,
		std::function<std::unique_ptr<Command>()>> factory {
			{ "ViewingGroup", [] { return std::make_unique<DrawingInstructions::ViewingGroup>(); } },
			{ "DisplayPlane", [] { return std::make_unique<DrawingInstructions::DisplayPlane>(); } },
			{ "DrawingPriority", [] { return std::make_unique<DrawingInstructions::DrawingPriority>(); } },
			{ "ScaleMinimum", [] { return std::make_unique<DrawingInstructions::ScaleMinimum>(); } },
			{ "ScaleMaximum", [] { return std::make_unique<DrawingInstructions::ScaleMaximum>(); } },
			{ "Id", [] { return std::make_unique<DrawingInstructions::Id>(); } },
			{ "Parent", [] { return std::make_unique<DrawingInstructions::Parent>(); } },
			{ "Hover", [] { return std::make_unique<DrawingInstructions::Hover>(); } },
			{ "LocalOffset", [] { return std::make_unique<DrawingInstructions::LocalOffset>(); } },
			{ "LinePlacement", [] { return std::make_unique<DrawingInstructions::LinePlacement>(); } },
			{ "AreaPlacement", [] { return std::make_unique<DrawingInstructions::AreaPlacement>(); } },
			{ "AreaCRS", [] { return std::make_unique<DrawingInstructions::AreaCRS>(); } },
			{ "Rotation", [] { return std::make_unique<DrawingInstructions::Rotation>(); } },
			{ "ScaleFactor", [] { return std::make_unique<DrawingInstructions::ScaleFactor>(); } },
			{ "LineStyle", [] { return std::make_unique<DrawingInstructions::LineStyle>(); } },
			{ "LineSymbol", [] { return std::make_unique<DrawingInstructions::LineSymbol>(); } },
			{ "Dash", [] { return std::make_unique<DrawingInstructions::Dash>(); } },
			{ "FontColor", [] { return std::make_unique<DrawingInstructions::FontColor>(); } },
			{ "FontSize", [] { return std::make_unique<DrawingInstructions::FontSize>(); } },
			{ "FontProportion", [] { return std::make_unique<DrawingInstructions::FontProportion>(); } },
			{ "FontWeight", [] { return std::make_unique<DrawingInstructions::FontWeight>(); } },
			{ "FontSlant", [] { return std::make_unique<DrawingInstructions::FontSlant>(); } },
			{ "FontSerifs", [] { return std::make_unique < DrawingInstructions::FontSerifs>(); } },
			{ "FontUnderline", [] { return std::make_unique<DrawingInstructions::FontUnderline>(); } },
			{ "FontStrikethrough", [] { return std::make_unique<DrawingInstructions::FontStrikethrough>(); } },
			{ "FontUpperline", [] { return std::make_unique<DrawingInstructions::FontUpperline>(); } },
			{ "FontReference", [] { return std::make_unique<DrawingInstructions::FontReference>(); } },
			{ "TextAlignHorizontal", [] { return std::make_unique<DrawingInstructions::TextAlignHorizontal>(); } },
			{ "TextAlignVertical", [] { return std::make_unique<DrawingInstructions::TextAlignVertical>(); } },
			{ "TextVerticalOffset", [] { return std::make_unique<DrawingInstructions::TextVerticalOffset>(); } },
			{ "OverrideColor", [] { return std::make_unique<DrawingInstructions::OverrideColor>(); } },
			{ "OverrideAll", [] { return std::make_unique<DrawingInstructions::OverrideAll>(); } },
			{ "SpatialReference", [] { return std::make_unique<DrawingInstructions::SpatialReference>(); } },
			{ "AugmentedPoint", [] { return std::make_unique<DrawingInstructions::AugmentedPoint>(); } },
			{ "AugmentedRay", [] { return std::make_unique<DrawingInstructions::AugmentedRay>(); } },
			{ "AugmentedPath", [] { return std::make_unique<DrawingInstructions::AugmentedPath>(); } },
			{ "Polyline", [] { return std::make_unique<DrawingInstructions::Polyline>(); } },
			{ "Arc3Points", [] { return std::make_unique<DrawingInstructions::Arc3Points>(); } },
			{ "ArcByRadius", [] { return std::make_unique<DrawingInstructions::ArcByRadius>(); } },
			{ "Annulus", [] { return std::make_unique<DrawingInstructions::Annulus>(); } },
			{ "ClearGeometry", [] { return std::make_unique<DrawingInstructions::ClearGeometry>(); } },
			{ "LookupEntry", [] { return std::make_unique<DrawingInstructions::LookupEntry>(); } },
			{ "NumericAnnotation", [] { return std::make_unique<DrawingInstructions::NumericAnnotation>(); } },
			{ "SymbolAnnotation", [] { return std::make_unique<DrawingInstructions::SymbolAnnotation>(); } },
			{ "CoverageColor", [] { return std::make_unique<DrawingInstructions::CoverageColor>(); } },
			{ "Date", [] { return std::make_unique<DrawingInstructions::Date>(); } },
			{ "Time", [] { return std::make_unique<DrawingInstructions::Time>(); } },
			{ "DateTime", [] { return std::make_unique<DrawingInstructions::DateTime>(); } },
			{ "TimeValid", [] { return std::make_unique<DrawingInstructions::TimeValid>(); } },
			{ "ClearTime", [] { return std::make_unique<DrawingInstructions::ClearTime>(); } },
			{ "AlertReference", [] { return std::make_unique<DrawingInstructions::AlertReference>(); } },
			{ "PointInstruction", [] { return std::make_unique<DrawingInstructions::PointInstruction>(); } },
			{ "LineInstruction", [] { return std::make_unique<DrawingInstructions::LineInstruction>(); } },
			{ "LineInstructionUnsuppressed", [] { return std::make_unique<DrawingInstructions::LineInstructionUnsuppressed>(); } },
			{ "ColorFill", [] { return std::make_unique<DrawingInstructions::ColorFill>(); } },
			{ "AreaFillReference", [] { return std::make_unique<DrawingInstructions::AreaFillReference>(); } },
			{ "PixmapFill", [] { return std::make_unique<DrawingInstructions::PixmapFill>(); } },
			{ "SymbolFill", [] { return std::make_unique<DrawingInstructions::SymbolFill>(); } },
			{ "HatchFill", [] { return std::make_unique<DrawingInstructions::HatchFill>(); } },
			{ "TextInstruction", [] { return std::make_unique<DrawingInstructions::TextInstruction>(); } },
			{ "CoverageFill", [] { return std::make_unique<DrawingInstructions::CoverageFill>(); } },
			{ "NullInstruction", [] { return std::make_unique<DrawingInstructions::NullInstruction>(); } }
	};

	auto it = factory.find(command);
	if (it == factory.end()) 
	{
		return;
	} 

	auto obj = it->second();

	obj->parse(params);

	Insert(std::move(obj));
}


void CommandList::Insert(std::unique_ptr<DrawingInstructions::Command> command)
{
	commands.push_back(std::move(command));
}

std::list<S100_Instruction*> CommandList::Parse()
{
	for (auto i = commands.begin(); i != commands.end(); ++i)
	{
		
	}
}