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

void CommandList::Insert(std::string id, std::string& command, std::string& params)
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

	obj->setId(id);
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

	// State commands
	// Applicability: All drawing commands
	std::optional<Part9a::ViewingGroup> viewingGroup;
	std::optional<Part9a::Id> id;
	std::optional<Part9a::SpatialReference> spatialReference;
	std::optional<Part9a::TimeValid> timeValid;
	std::optional<Part9a::AlertReference> alertReference;

	// Applicability: All drawing commands, except NullInstruction
	std::optional<Part9a::DisplayPlane> displayPlane;
	std::optional<Part9a::DrawingPriority> drawingPriority;
	std::optional<Part9a::ScaleMinimum> scaleMinimum;
	std::optional<Part9a::ScaleMaximum> scaleMaximum;
	std::optional<Part9a::Parent> parent;
	std::optional<Part9a::Hover> hover;

	// Applicability: PointInstruction, SymbolFill, TextInstruction
	std::optional<Part9a::LocalOffset> localOffset;

	// Applicability: PointInstruction, TextInstruction
	std::optional<Part9a::LinePlacement> linePlacement;
	std::optional<Part9a::AreaPlacement> areaPlacement;

	// Applicability: AreaFillReference, PixmapFill, SymbolFill, HatchFill, TextInstruction
	std::optional<Part9a::AreaCRS> areaCRS;

	// Applicability: PointInstruction, SymbolFill, TextInstruction, CoverageFill
	std::optional<Part9a::Rotation> rotation;
	std::optional<Part9a::Rotation> scaleFactor;

	// Applicability: LineStyle
	std::optional<Part9a::Dash> dash;
	std::optional<Part9a::LineSymbol> lineSymbol;
	
	// Applicability: LineInstruction, LineInstructionUnsuppressed, HatchFill
	std::optional<Part9a::LineStyle> lineStyle;

	// Applicability: TextInstruction
	std::optional<Part9a::FontColor> fontColor;

	// Applicability: TextInstruction, CoverageFill
	std::optional<Part9a::FontBackgroundColor> fontBackgroundColor;
	std::optional<Part9a::FontSize> fontSize;
	std::optional<Part9a::FontProportion> fontProportion;
	std::optional<Part9a::FontWeight> fontWeight;
	std::optional<Part9a::FontSlant> fontSlant;
	std::optional<Part9a::FontSerifs> fontSerifs;

	// Applicability: TextInstruction
	std::optional<Part9a::FontUnderline> fontUnderline;
	std::optional<Part9a::FontStrikethrough> fontStrikethrough;
	std::optional<Part9a::FontUpperline> fontUpperline;
	std::optional<Part9a::FontReference> fontReference;
	std::optional<Part9a::TextAlignHorizontal> textAlignHorizontal;
	std::optional<Part9a::TextAlignVertical> textAlignVertical;
	std::optional<Part9a::TextVerticalOffset> textVerticalOffset;

	// Applicability: PointInstruction, AreaFillReference, PixmapFill, SymbolFill
	std::optional<Part9a::OverrideColor> overrideColor;
	std::optional<Part9a::OverrideAll> overrideAll;
	std::optional<Part9a::ClearOverride> clearOverride;

	// Applicability: PointInstruction, TextInstruction, NullInstruction
	std::optional<Part9a::AugmentedPoint> augmentedPoint;

	// Applicability: LineInstruction, LineInstructionUnsuppressed, TextInstruction, NullInstruction
	std::optional<Part9a::AugmentedRay> augmentedRay;

	// Applicability: All drawing commands except PointInstruction
	std::optional<Part9a::AugmentedPath> augmentedPath;

	// Applicability: AugmentedPath
	std::optional<Part9a::Polyline> polyline;
	std::optional<Part9a::Arc3Points> arc3Points;
	std::optional<Part9a::ArcByRadius> arcByRadius;
	std::optional<Part9a::Annulus> annulus;

	// Applicability: AugmentedPath, SpatialReference
	std::optional<Part9a::ClearGeometry> clearGeometry;

	// Applicability: LookupEntry
	std::optional<Part9a::NumericAnnotation> numericAnnotation;
	std::optional<Part9a::SymbolAnnotation> symbolAnnotation;
	std::optional<Part9a::CoverageColor> coverageColor;

	// Applicability: CoverageFill
	std::optional<Part9a::LookupEntry> lookupEntry;

	// Applicability: TimeValid
	std::optional<Part9a::Date> date;
	std::optional<Part9a::Time> time;
	std::optional<Part9a::DateTime> dateTime;

	// Applicability: All time commands, all drawing commands
	std::optional<Part9a::ClearTime> clearTime;
	
	for (auto i = commands.begin(); i != commands.end(); ++i) {
		auto command = i->get();

		// PointInstruction
		if (dynamic_cast<Part9a::PointInstruction*>(command)) {
			auto ptr = dynamic_cast<Part9a::PointInstruction*>(command);
			auto instruction = new S100_PointInstruction();
			instruction->SetFeatureReference(ptr->getId());
			instruction->SetViewingGroup(viewingGroup.has_value() ? viewingGroup->GetViewingGroups() : std::vector<std::string>());
			result.push_back(instruction);
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
		else if (dynamic_cast<Part9a::ViewingGroup*>(command)) {
			auto ptr = dynamic_cast<Part9a::ViewingGroup*>(command);
			viewingGroup = *ptr;
		}
		else if (dynamic_cast<Part9a::StateCommand*>(command)) {
			OutputDebugString(L"A");
		}
	}

	return result;
}