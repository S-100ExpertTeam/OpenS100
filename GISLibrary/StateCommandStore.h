#pragma once

#include "AlertCommands.h"
#include "ColourOverrideCommands.h"
#include "CoverageCommands.h"
#include "GeometryCommands.h"
#include "LineStyleCommands.h"
#include "TextStyleCommands.h"
#include "TimeCommands.h"
#include "TransformCommands.h"
#include "VisibilityCommands.h"

#include <optional>

class StateCommandStore
{
public:
	StateCommandStore() = default;
	virtual ~StateCommandStore() = default;

public:
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
	std::optional<Part9a::ScaleFactor> scaleFactor;

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

public:
	bool SetStateCommand(Part9a::StateCommand* command);
};

