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
#include "..\\PortrayalCatalogue\\S100_PointInstruction.h"
#include "..\\PortrayalCatalogue\\S100_LineInstruction.h"


#include <unordered_map>
#include <memory>

void CommandList::Insert(std::string& command, std::string& params)
{
	static const std::unordered_map<std::string,
		std::function<std::unique_ptr<Part9a::Command>()>> factory {
			{ "ViewingGroup", [] { return std::make_unique<Part9a::ViewingGroup>(); } },
			{ "DisplayPlane", [] { return std::make_unique<Part9a::DisplayPlane>(); } },
			{ "DrawingPriority", [] { return std::make_unique<Part9a::DrawingPriority>(); } },
			{ "ScaleMinimum", [] { return std::make_unique<Part9a::ScaleMinimum>(); } },
			{ "ScaleMaximum", [] { return std::make_unique<Part9a::ScaleMaximum>(); } },
			{ "Id", [] { return std::make_unique<Part9a::Id>(); } },
			{ "Parent", [] { return std::make_unique<Part9a::Parent>(); } },
			{ "Hover", [] { return std::make_unique<Part9a::Hover>(); } },
			{ "LocalOffset", [] { return std::make_unique<Part9a::LocalOffset>(); } },
			{ "LinePlacement", [] { return std::make_unique<Part9a::LinePlacement>(); } },
			{ "AreaPlacement", [] { return std::make_unique<Part9a::AreaPlacement>(); } },
			{ "AreaCRS", [] { return std::make_unique<Part9a::AreaCRS>(); } },
			{ "Rotation", [] { return std::make_unique<Part9a::Rotation>(); } },
			{ "ScaleFactor", [] { return std::make_unique<Part9a::ScaleFactor>(); } },
			{ "LineStyle", [] { return std::make_unique<Part9a::LineStyle>(); } },
			{ "LineSymbol", [] { return std::make_unique<Part9a::LineSymbol>(); } },
			{ "Dash", [] { return std::make_unique<Part9a::Dash>(); } },
			{ "FontColor", [] { return std::make_unique<Part9a::FontColor>(); } },
			{ "FontSize", [] { return std::make_unique<Part9a::FontSize>(); } },
			{ "FontProportion", [] { return std::make_unique<Part9a::FontProportion>(); } },
			{ "FontWeight", [] { return std::make_unique<Part9a::FontWeight>(); } },
			{ "FontSlant", [] { return std::make_unique<Part9a::FontSlant>(); } },
			{ "FontSerifs", [] { return std::make_unique < Part9a::FontSerifs>(); } },
			{ "FontUnderline", [] { return std::make_unique<Part9a::FontUnderline>(); } },
			{ "FontStrikethrough", [] { return std::make_unique<Part9a::FontStrikethrough>(); } },
			{ "FontUpperline", [] { return std::make_unique<Part9a::FontUpperline>(); } },
			{ "FontReference", [] { return std::make_unique<Part9a::FontReference>(); } },
			{ "TextAlignHorizontal", [] { return std::make_unique<Part9a::TextAlignHorizontal>(); } },
			{ "TextAlignVertical", [] { return std::make_unique<Part9a::TextAlignVertical>(); } },
			{ "TextVerticalOffset", [] { return std::make_unique<Part9a::TextVerticalOffset>(); } },
			{ "OverrideColor", [] { return std::make_unique<Part9a::OverrideColor>(); } },
			{ "OverrideAll", [] { return std::make_unique<Part9a::OverrideAll>(); } },
			{ "SpatialReference", [] { return std::make_unique<Part9a::SpatialReference>(); } },
			{ "AugmentedPoint", [] { return std::make_unique<Part9a::AugmentedPoint>(); } },
			{ "AugmentedRay", [] { return std::make_unique<Part9a::AugmentedRay>(); } },
			{ "AugmentedPath", [] { return std::make_unique<Part9a::AugmentedPath>(); } },
			{ "Polyline", [] { return std::make_unique<Part9a::Polyline>(); } },
			{ "Arc3Points", [] { return std::make_unique<Part9a::Arc3Points>(); } },
			{ "ArcByRadius", [] { return std::make_unique<Part9a::ArcByRadius>(); } },
			{ "Annulus", [] { return std::make_unique<Part9a::Annulus>(); } },
			{ "ClearGeometry", [] { return std::make_unique<Part9a::ClearGeometry>(); } },
			{ "LookupEntry", [] { return std::make_unique<Part9a::LookupEntry>(); } },
			{ "NumericAnnotation", [] { return std::make_unique<Part9a::NumericAnnotation>(); } },
			{ "SymbolAnnotation", [] { return std::make_unique<Part9a::SymbolAnnotation>(); } },
			{ "CoverageColor", [] { return std::make_unique<Part9a::CoverageColor>(); } },
			{ "Date", [] { return std::make_unique<Part9a::Date>(); } },
			{ "Time", [] { return std::make_unique<Part9a::Time>(); } },
			{ "DateTime", [] { return std::make_unique<Part9a::DateTime>(); } },
			{ "TimeValid", [] { return std::make_unique<Part9a::TimeValid>(); } },
			{ "ClearTime", [] { return std::make_unique<Part9a::ClearTime>(); } },
			{ "AlertReference", [] { return std::make_unique<Part9a::AlertReference>(); } },
			{ "PointInstruction", [] { return std::make_unique<Part9a::PointInstruction>(); } },
			{ "LineInstruction", [] { return std::make_unique<Part9a::LineInstruction>(); } },
			{ "LineInstructionUnsuppressed", [] { return std::make_unique<Part9a::LineInstructionUnsuppressed>(); } },
			{ "ColorFill", [] { return std::make_unique<Part9a::ColorFill>(); } },
			{ "AreaFillReference", [] { return std::make_unique<Part9a::AreaFillReference>(); } },
			{ "PixmapFill", [] { return std::make_unique<Part9a::PixmapFill>(); } },
			{ "SymbolFill", [] { return std::make_unique<Part9a::SymbolFill>(); } },
			{ "HatchFill", [] { return std::make_unique<Part9a::HatchFill>(); } },
			{ "TextInstruction", [] { return std::make_unique<Part9a::TextInstruction>(); } },
			{ "CoverageFill", [] { return std::make_unique<Part9a::CoverageFill>(); } },
			{ "NullInstruction", [] { return std::make_unique<Part9a::NullInstruction>(); } }
	};

	auto it = factory.find(command);
	if (it == factory.end()) 
	{
		return;
	} 

	auto obj = it->second();

	obj->setParameter(params);
	obj->parse(params);

	Insert(std::move(obj));
}


void CommandList::Insert(std::unique_ptr<Part9a::Command> command)
{
	commands.push_back(std::move(command));
}

std::list<S100_Instruction*> CommandList::Parse()
{
	std::list<S100_Instruction*> result;
	std::list<Part9a::Command*> stateCommand; // Context;

	for (auto i = commands.begin(); i != commands.end(); ++i) {
		auto command = i->get();

		// PointInstruction
		if (dynamic_cast<Part9a::PointInstruction*>(command)) {
			auto ptr = dynamic_cast<Part9a::PointInstruction*>(command);
			auto instruction = new S100_PointInstruction();
			continue;
		}
		else if (dynamic_cast<Part9a::LineInstruction*>(command)) {

		}
		else if (dynamic_cast<Part9a::LineInstructionUnsuppressed*>(command)) {

		}
		else if (dynamic_cast<Part9a::ColorFill*>(command)) {
		}
		else if (dynamic_cast<Part9a::AreaFillReference*>(command)) {
		}
		else if (dynamic_cast<Part9a::PixmapFill*>(command)) {
		}
		else if (dynamic_cast<Part9a::SymbolFill*>(command)) {
		}
		else if (dynamic_cast<Part9a::HatchFill*>(command)) {
		}
		else if (dynamic_cast<Part9a::TextInstruction*>(command)) {
		}
		else if (dynamic_cast<Part9a::CoverageFill*>(command)) {
		}
		else if (dynamic_cast<Part9a::NullInstruction*>(command)) {
		}

	}

	return result;
}