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
#include "PointInstruction.h"

#include "..\\PortrayalCatalogue\\S100_Instruction.h"

#include <unordered_map>
#include <memory>

void CommandList::Insert(std::string& command, std::string& params)
{
	static const std::unordered_map<std::string,
		std::function<std::unique_ptr<DrawingCommand::Command>()>> factory {
			{ "ViewingGroup", [] { return std::make_unique<DrawingCommand::ViewingGroup>(); } },
			{ "DisplayPlane", [] { return std::make_unique<DrawingCommand::DisplayPlane>(); } },
			{ "DrawingPriority", [] { return std::make_unique<DrawingCommand::DrawingPriority>(); } },
			{ "ScaleMinimum", [] { return std::make_unique<DrawingCommand::ScaleMinimum>(); } },
			{ "ScaleMaximum", [] { return std::make_unique<DrawingCommand::ScaleMaximum>(); } },
			{ "Id", [] { return std::make_unique<DrawingCommand::Id>(); } },
			{ "Parent", [] { return std::make_unique<DrawingCommand::Parent>(); } },
			{ "Hover", [] { return std::make_unique<DrawingCommand::Hover>(); } },
			{ "LocalOffset", [] { return std::make_unique<DrawingCommand::LocalOffset>(); } },
			{ "LinePlacement", [] { return std::make_unique<DrawingCommand::LinePlacement>(); } },
			{ "AreaPlacement", [] { return std::make_unique<DrawingCommand::AreaPlacement>(); } },
			{ "AreaCRS", [] { return std::make_unique<DrawingCommand::AreaCRS>(); } },
			{ "Rotation", [] { return std::make_unique<DrawingCommand::Rotation>(); } },
			{ "ScaleFactor", [] { return std::make_unique<DrawingCommand::ScaleFactor>(); } },
			{ "LineStyle", [] { return std::make_unique<DrawingCommand::LineStyle>(); } },
			{ "LineSymbol", [] { return std::make_unique<DrawingCommand::LineSymbol>(); } },
			{ "Dash", [] { return std::make_unique<DrawingCommand::Dash>(); } },
			{ "FontColor", [] { return std::make_unique<DrawingCommand::FontColor>(); } },
			{ "FontSize", [] { return std::make_unique<DrawingCommand::FontSize>(); } },
			{ "FontProportion", [] { return std::make_unique<DrawingCommand::FontProportion>(); } },
			{ "FontWeight", [] { return std::make_unique<DrawingCommand::FontWeight>(); } },
			{ "FontSlant", [] { return std::make_unique<DrawingCommand::FontSlant>(); } },
			{ "FontSerifs", [] { return std::make_unique < DrawingCommand::FontSerifs>(); } },
			{ "FontUnderline", [] { return std::make_unique<DrawingCommand::FontUnderline>(); } },
			{ "FontStrikethrough", [] { return std::make_unique<DrawingCommand::FontStrikethrough>(); } },
			{ "FontUpperline", [] { return std::make_unique<DrawingCommand::FontUpperline>(); } },
			{ "FontReference", [] { return std::make_unique<DrawingCommand::FontReference>(); } },
			{ "TextAlignHorizontal", [] { return std::make_unique<DrawingCommand::TextAlignHorizontal>(); } },
			{ "TextAlignVertical", [] { return std::make_unique<DrawingCommand::TextAlignVertical>(); } },
			{ "TextVerticalOffset", [] { return std::make_unique<DrawingCommand::TextVerticalOffset>(); } },
			{ "OverrideColor", [] { return std::make_unique<DrawingCommand::OverrideColor>(); } },
			{ "OverrideAll", [] { return std::make_unique<DrawingCommand::OverrideAll>(); } },
			{ "SpatialReference", [] { return std::make_unique<DrawingCommand::SpatialReference>(); } },
			{ "AugmentedPoint", [] { return std::make_unique<DrawingCommand::AugmentedPoint>(); } },
			{ "AugmentedRay", [] { return std::make_unique<DrawingCommand::AugmentedRay>(); } },
			{ "AugmentedPath", [] { return std::make_unique<DrawingCommand::AugmentedPath>(); } },
			{ "Polyline", [] { return std::make_unique<DrawingCommand::Polyline>(); } },
			{ "Arc3Points", [] { return std::make_unique<DrawingCommand::Arc3Points>(); } },
			{ "ArcByRadius", [] { return std::make_unique<DrawingCommand::ArcByRadius>(); } },
			{ "Annulus", [] { return std::make_unique<DrawingCommand::Annulus>(); } },
			{ "ClearGeometry", [] { return std::make_unique<DrawingCommand::ClearGeometry>(); } },
			{ "LookupEntry", [] { return std::make_unique<DrawingCommand::LookupEntry>(); } },
			{ "NumericAnnotation", [] { return std::make_unique<DrawingCommand::NumericAnnotation>(); } },
			{ "SymbolAnnotation", [] { return std::make_unique<DrawingCommand::SymbolAnnotation>(); } },
			{ "CoverageColor", [] { return std::make_unique<DrawingCommand::CoverageColor>(); } },
			{ "Date", [] { return std::make_unique<DrawingCommand::Date>(); } },
			{ "Time", [] { return std::make_unique<DrawingCommand::Time>(); } },
			{ "DateTime", [] { return std::make_unique<DrawingCommand::DateTime>(); } },
			{ "TimeValid", [] { return std::make_unique<DrawingCommand::TimeValid>(); } },
			{ "ClearTime", [] { return std::make_unique<DrawingCommand::ClearTime>(); } },
			{ "AlertReference", [] { return std::make_unique<DrawingCommand::AlertReference>(); } },
			{ "PointInstruction", [] { return std::make_unique<DrawingCommand::PointInstruction>(); } },
			{ "LineInstruction", [] { return std::make_unique<DrawingCommand::LineInstruction>(); } },
			{ "LineInstructionUnsuppressed", [] { return std::make_unique<DrawingCommand::LineInstructionUnsuppressed>(); } },
			{ "ColorFill", [] { return std::make_unique<DrawingCommand::ColorFill>(); } },
			{ "AreaFillReference", [] { return std::make_unique<DrawingCommand::AreaFillReference>(); } },
			{ "PixmapFill", [] { return std::make_unique<DrawingCommand::PixmapFill>(); } },
			{ "SymbolFill", [] { return std::make_unique<DrawingCommand::SymbolFill>(); } },
			{ "HatchFill", [] { return std::make_unique<DrawingCommand::HatchFill>(); } },
			{ "TextInstruction", [] { return std::make_unique<DrawingCommand::TextInstruction>(); } },
			{ "CoverageFill", [] { return std::make_unique<DrawingCommand::CoverageFill>(); } },
			{ "NullInstruction", [] { return std::make_unique<DrawingCommand::NullInstruction>(); } }
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


void CommandList::Insert(std::unique_ptr<DrawingCommand::Command> command)
{
	commands.push_back(std::move(command));
}

std::list<S100_Instruction*> CommandList::Parse()
{
	std::list<S100_Instruction*> result;

	for (auto i = commands.begin(); i != commands.end(); ++i)
	{
		


		// PointInstruction
		// -- 

		// LineInstruction Or LineInstructionUnsuppressed
		// refers to a line style in PC or defiend by LineStyle command

		// ColorFill
	}

	return result;
}